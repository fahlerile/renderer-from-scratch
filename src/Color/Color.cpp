#include <cmath>
#include "Color.hpp"

Color::Color() { }

Color::Color(unsigned char red, unsigned char green, unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Color Color::operator+(Color other)
{
    return {(unsigned char) ((unsigned int) this->red   + (unsigned int) other.red),
            (unsigned char) ((unsigned int) this->green + (unsigned int) other.green),
            (unsigned char) ((unsigned int) this->blue  + (unsigned int) other.blue)};
}

Color Color::operator-(Color other)
{
    return {(unsigned char) ((unsigned int) this->red   - (unsigned int) other.red),
            (unsigned char) ((unsigned int) this->green - (unsigned int) other.green),
            (unsigned char) ((unsigned int) this->blue  - (unsigned int) other.blue)};
}

Color Color::operator*(double scalar)
{
    return {(unsigned char) std::round((double) this->red   * scalar),
            (unsigned char) std::round((double) this->green * scalar),
            (unsigned char) std::round((double) this->blue  * scalar)};
}

Color Color::operator/(double scalar)
{
    return {(unsigned char) std::round((double) this->red   / scalar),
            (unsigned char) std::round((double) this->green / scalar),
            (unsigned char) std::round((double) this->blue  / scalar)};
}

Color Color::operator+(SignedColor other)
{
    return {(unsigned char) ((unsigned int) this->red   + other.red),
            (unsigned char) ((unsigned int) this->green + other.green),
            (unsigned char) ((unsigned int) this->blue  + other.blue)};
}

Color::operator SignedColor()
{
    return {(short) this->red,
            (short) this->green,
            (short) this->blue};
}

SignedColor SignedColor::operator-(SignedColor other)
{
    return {
        (short) (this->red - other.red),
        (short) (this->green - other.green),
        (short) (this->blue - other.blue)
    };
}

SignedColor SignedColor::operator/(double scalar)
{
    return {
        (short) std::round(this->red / scalar),
        (short) std::round(this->green / scalar),
        (short) std::round(this->blue / scalar)
    };
}
