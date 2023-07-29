#include <iostream>

#include "Image/Image.hpp"

int main()
{
    unsigned int size = 100;
    Image image = Image(size, size);

    image.line(50, 10, 90, 30, Color(255, 255, 255));

    image.save("test.ppm");
}