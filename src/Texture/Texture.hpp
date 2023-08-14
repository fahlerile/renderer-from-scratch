#pragma once
#include "utils/vec/vec2.hpp"
#include "utils/Color/Color.hpp"

class Texture
{
public:
    // Supports only PPM images
    Texture(std::string path);

    Color get_color_from_uv(vec2d uv);

private:
    std::vector<std::vector<Color>> data;
    int width;
    int height;
};