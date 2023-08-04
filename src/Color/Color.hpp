#pragma once

// not unsigned char here because of `Window::line()` (color_delta)
struct Color
{
    short red = 0;
    short green = 0;
    short blue = 0;

    Color operator*(double scalar);
    Color operator/(double scalar);
    Color operator+(Color other);
    Color operator+=(Color other);
    Color operator-(Color other);
};
