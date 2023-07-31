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
};

class Image
{
public:
    Image(vec2i dimensions);

    void set(vec2i position, Color color);
    void line(vec2i pos1, vec2i pos2, Color color);
    void triangle(vec2i v0, vec2i v1, vec2i v2, Color color);
    void flip_vertically();
    void save(std::string path);

private:
    std::vector<std::vector<Color>> data;
    vec2i dimensions;
};