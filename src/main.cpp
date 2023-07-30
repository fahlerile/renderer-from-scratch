#include <iostream>
#include <vector>

#include "Image/Image.hpp"
#include "Model/Model.hpp"

int main()
{
    vec2i dimensions = {50, 50};

    Image image = Image(dimensions);

    vec2i vertices[3] = {
        {10, 10},
        {40, 15},
        {30, 40}
    };
    image.triangle(vertices, {255, 255, 255});

    image.flip_vertically();
    image.save("render.ppm");
}