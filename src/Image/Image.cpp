#include <fstream>
#include <stdexcept>
#include <cmath>

#include <iostream>  // debug

#include "Image.hpp"

Color::Color() { }

Color::Color(unsigned char red, unsigned char green, unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Image::Image(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    this->data = new Color*[height];
    for (int i = 0; i < height; i++)
    {
        this->data[i] = new Color[width];
    }
}

Image::~Image()
{
    for (int i = 0; i < this->height; i++)
    {
        delete[] this->data[i];
    }
    delete[] this->data;
}

void Image::set(unsigned int x, unsigned int y, Color color)
{
    if (x >= this->width || y >= this->height)
        throw std::invalid_argument("Out of bounds set() (x=" + std::to_string(x) + ", y=" + std::to_string(y) + ")");
    this->data[y][x] = color;
}

// Bresenham's algorithm
// See https://www.ics.uci.edu/~gopi/CS112/web/handouts/OldFiles/Bresenham.pdf
void Image::line(int x0, int y0, int x1, int y1, Color color)
{
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

    std::cout << "dx: " << dx << "\ndy: " << dy << std::endl; 

    int x = x0;
    int y = y0;

    int D = 2 * dy - dx;

    if (!steep)
        this->set(x, y, color);
    else  // if steep, de-transpose
        this->set(y, x, color);

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
            this->set(x, y, color);
        else  // if steep, de-transpose
            this->set(y, x, color);
    }
}

// Save the image to PPM format
void Image::save(std::string path)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file << "P6\n" << "# \n" << this->width << " " << this->height << "\n255\n";
        for (int i = 0; i < this->height; i++)
        {
            for (int j = 0; j < this->width; j++)
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