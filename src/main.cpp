#include <iostream>
#include <vector>

#include "Image/Image.hpp"
#include "Model/Model.hpp"

int main()
{
    vec2i dimensions = {100, 100};

    Image image = Image(dimensions);

    vec2i vertices[4] = {
        {49, 19},
        {19, 79},
        {79, 49},
        {79, 89}
    };
    image.triangle(vertices[0], vertices[1], vertices[2],
                   {255, 0, 0}, {0, 255, 0}, {0, 0, 255});
    image.save("image1.ppm");
    image.triangle(vertices[2], vertices[1], vertices[3],
                   {255, 255, 0}, {255, 255, 0}, {255, 255, 0});
    image.save("image2.ppm");
}