#include <iostream>
#include <vector>

#include "Image/Image.hpp"

int main()
{
    unsigned int size = 100;

    std::vector<std::vector<int>> test_cases = {
        {0, 0, 20, 10},   // (slope < 1)  // PASS
        {0, 0, 25, 25},   // (slope = 1)  // PASS
        {0, 0, 25, 50},   // (slope > 1)  // PASS
        {0, 49, 20, 10},  // (slope < 1)  // PASS
        {0, 20, 20, 0},   // (slope = 1)  // PASS
        {0, 49, 20, 0}    // (slope > 1)  // PASS
    };

    int num = 0;
    for (auto pts : test_cases)
    {
        Image image = Image(size, size);
        image.line(pts[0], pts[1], pts[2], pts[3], Color(255, 255, 255));
        image.save(std::to_string(num) + ".ppm");
        num++;
    }
}