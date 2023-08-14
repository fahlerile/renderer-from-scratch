#pragma once

struct Color
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    Color operator*(double scalar);
    Color operator/(double scalar);
    Color operator+(Color other);
    Color operator+=(Color other);
    Color operator-(Color other);
};
