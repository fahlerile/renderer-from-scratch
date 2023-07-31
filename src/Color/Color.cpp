#include "Color.hpp"

Color::Color() { }

Color::Color(unsigned char red, unsigned char green, unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Color Color::operator*(float scalar)
{
    return {(unsigned char) ((float) this->red   * scalar),
            (unsigned char) ((float) this->green * scalar),
            (unsigned char) ((float) this->blue  * scalar)};
}

Color Color::operator+(Color other)
{
    return {(unsigned char) ((unsigned int) this->red   + (unsigned int) other.red),
            (unsigned char) ((unsigned int) this->green + (unsigned int) other.green),
            (unsigned char) ((unsigned int) this->blue  + (unsigned int) other.blue)};
}