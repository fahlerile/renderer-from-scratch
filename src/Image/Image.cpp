#include <fstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <algorithm>

#include "Image.hpp"
#include "utils/vec.hpp"

Color::Color() { }

Color::Color(unsigned char red, unsigned char green, unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Image::Image(vec2i dimensions)
{
    this->dimensions = dimensions;
    for (int i = 0; i < dimensions.y; i++)
    {
        std::vector<Color> row;
        for (int j = 0; j < dimensions.x; j++)
            row.push_back(Color());
        this->data.push_back(row);
    }
}

void Image::set(vec2i position, Color color)
{
    if (position.x >= this->dimensions.x || position.y >= this->dimensions.y)
        throw std::invalid_argument("Out of bounds set() (x=" + std::to_string(position.x) + ", y=" + std::to_string(position.y) + ")");
    this->data[position.y][position.x] = color;
}

// Bresenham's algorithm
// See https://www.ics.uci.edu/~gopi/CS112/web/handouts/OldFiles/Bresenham.pdf
void Image::line(vec2i pos0, vec2i pos1, Color color)
{
    int x0 = pos0.x;
    int y0 = pos0.y;
    int x1 = pos1.x;
    int y1 = pos1.y;

    bool steep = false;
    if (abs(x1-x0) < abs(y1-y0))  // if slope > 1, transpose
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1)  // need to render from left to right
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int step = (y0 > y1) ? -1 : 1;

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    int x = x0;
    int y = y0;

    int D = 2 * dy - dx;

    if (!steep)
        this->set({x, y}, color);
    else  // if steep, de-transpose
        this->set({y, x}, color);

    while (x < x1)
    {
        x++;
        if (D < 0)
            D += (2 * dy);
        else
        {
            y += step;
            D += (2 * (dy - dx));
        }
        if (!steep)
            this->set({x, y}, color);
        else  // if steep, de-transpose
            this->set({y, x}, color);
    }
}

// `vertices` is a pointer to an array of 3 vertices that are in counter-clockwise direction
void Image::triangle(vec2i* vertices, Color color)
{
    auto edge_function([](vec2i a, vec2i b, vec2i p)
    {
        vec2i ab = {b.x - a.x, b.y - a.y};
        vec2i ap = {p.x - a.x, p.y - a.y};
        return ab.x * ap.y - ab.y * ap.x;
    });

    vec2i v0 = vertices[0];
    vec2i v1 = vertices[1];
    vec2i v2 = vertices[2];

    int xmin = std::min(std::min(v0.x, v1.x), v2.x);
    int ymin = std::min(std::min(v0.y, v1.y), v2.y);
    int xmax = std::max(std::max(v0.x, v1.x), v2.x);
    int ymax = std::max(std::max(v0.y, v1.y), v2.y);

    for (int y = ymin; y < ymax; y++)
    {
        for (int x = xmin; x < xmax; x++)
        {
            vec2i p = {x, y};

            int w0 = edge_function(v0, v1, p);
            int w1 = edge_function(v1, v2, p);
            int w2 = edge_function(v2, v0, p);

            bool pixel_inside_triangle = w0 >= 0 && w1 >= 0 && w2 >= 0;

            if (pixel_inside_triangle)
                this->set({x, y}, color);
        }
    }
}

void Image::flip_vertically()
{
    std::reverse(this->data.begin(), this->data.end());
}

// Save the image to PPM format
void Image::save(std::string path)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file << "P6\n" << "# \n" << this->dimensions.x << " " << this->dimensions.y << "\n255\n";
        for (int i = 0; i < this->dimensions.y; i++)
        {
            for (int j = 0; j < this->dimensions.x; j++)
            {
                file << this->data[i][j].red
                     << this->data[i][j].green
                     << this->data[i][j].blue;
            }
        }
    }
    else
        throw std::runtime_error("Unable to open file " + path);
    file.close();
}