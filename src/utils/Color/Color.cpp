#include "Color.hpp"

Color Color::operator*(double scalar)
{
    return {(short) ((double) this->red   * scalar),
            (short) ((double) this->green * scalar),
            (short) ((double) this->blue  * scalar)};
}

Color Color::operator/(double scalar)
{
    return {(short) ((double) this->red   / scalar),
            (short) ((double) this->green / scalar),
            (short) ((double) this->blue  / scalar)};
}

Color Color::operator+(Color other)
{
    return {(short) (this->red   + other.red),
            (short) (this->green + other.green),
            (short) (this->blue  + other.blue)};
}

Color Color::operator+=(Color other)
{
    *this = {(short) (this->red   + other.red),
             (short) (this->green + other.green),
             (short) (this->blue  + other.blue)};
    return *this;
}

Color Color::operator-(Color other)
{
    return {(short) (this->red   - other.red),
            (short) (this->green - other.green),
            (short) (this->blue  - other.blue)};
}
