#include <fstream>
#include <stdexcept>

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
    this->data[y][x] = color;
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