#pragma once

struct Color
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    Color();
    Color(unsigned char red, unsigned char green, unsigned char blue);

    Color operator*(float scalar);
    Color operator+(Color other);
};
