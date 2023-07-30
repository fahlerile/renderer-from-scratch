#include <iostream>

#include "Image/Image.hpp"

int main()
{
    unsigned int size = 100;
    Image image = Image(size, size);

    // slope > 1

    image.line(49, 49, 70, 40, Color(255, 255, 255));

    image.save("test.ppm");
}