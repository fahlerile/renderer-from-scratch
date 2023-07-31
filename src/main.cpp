#include <iostream>
#include <vector>

#include "Image/Image.hpp"
#include "Model/Model.hpp"

int main()
{
    vec2i dimensions = {50, 50};

    Image image = Image(dimensions);

    vec2i vertices[4] = {
        {10, 10},
        {40, 25},
        {10, 25},
        {30, 40}
    };
    image.triangle(vertices[0], vertices[1], vertices[2], {255, 255, 255});
    image.save("render1.ppm");
    image.triangle(vertices[1], vertices[3], vertices[2], {255, 0, 0});
    image.save("render2.ppm");
}