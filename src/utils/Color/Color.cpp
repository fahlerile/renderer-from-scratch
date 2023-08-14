#include "Color.hpp"

Color Color::operator*(double scalar)
{
    return {(unsigned char) ((double) this->red   * scalar),
            (unsigned char) ((double) this->green * scalar),
            (unsigned char) ((double) this->blue  * scalar)};
}

Color Color::operator/(double scalar)
{
    return {(unsigned char) ((double) this->red   / scalar),
            (unsigned char) ((double) this->green / scalar),
            (unsigned char) ((double) this->blue  / scalar)};
}

Color Color::operator+(Color other)
{
    return {(unsigned char) (this->red   + other.red),
            (unsigned char) (this->green + other.green),
            (unsigned char) (this->blue  + other.blue)};
}

Color Color::operator+=(Color other)
{
    *this = {(unsigned char) (this->red   + other.red),
             (unsigned char) (this->green + other.green),
             (unsigned char) (this->blue  + other.blue)};
    return *this;
}

Color Color::operator-(Color other)
{
    return {(unsigned char) (this->red   - other.red),
            (unsigned char) (this->green - other.green),
            (unsigned char) (this->blue  - other.blue)};
}
