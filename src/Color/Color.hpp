#pragma once

struct SignedColor;

struct Color
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    Color();
    Color(unsigned char red, unsigned char green, unsigned char blue);

    Color operator+(Color other);
    Color operator-(Color other);

    Color operator*(double scalar);
    Color operator/(double scalar);

    Color operator+(SignedColor other);

    operator SignedColor();
};

// Used in `Window::line()` as color delta for interpolation
struct SignedColor
{
    short red = 0;
    short green = 0;
    short blue = 0;

    SignedColor operator-(SignedColor other);
    SignedColor operator/(double scalar);
};
