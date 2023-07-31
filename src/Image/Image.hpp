#pragma once
#include <string>
#include <vector>
#include "utils/vec.hpp"

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

class Image
{
public:
    Image(vec2i dimensions);

    void set(vec2i position, Color color);
    void line(vec2i pos1, vec2i pos2, Color color);
    void triangle(vec2i v0, vec2i v1, vec2i v2,
                  Color c0, Color c1, Color c2);
    void flip_vertically();
    void save(std::string path);

private:
    std::vector<std::vector<Color>> data;
    vec2i dimensions;
};