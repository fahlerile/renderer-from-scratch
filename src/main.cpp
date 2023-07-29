#include <iostream>

#include "Image/Image.hpp"

int main()
{
    unsigned int size = 255;
    Image image = Image(size, size);
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            image.set(x, y, Color(x, x, x));
        }
    }
    image.save("test.ppm");
}