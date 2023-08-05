#pragma once
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"

// Base light class.
class Light
{
public:
    Light(Color color);
    float calculate_intensity(vec3d normal);
    Color get_color();

protected:
    Color color;
};
