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