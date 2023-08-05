#pragma once
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"

// Base light class.
class Light
{
public:
    Light(Color color);
    Color get_color();
    virtual float calculate_intensity(vec3d normal) = 0;

protected:
    Color color;
};
