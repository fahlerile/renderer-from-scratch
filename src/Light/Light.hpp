#pragma once
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"
#include "utils/mat/mat.hpp"

// Base light class.
class Light
{
public:
    Light(Color color);
    Color get_color();
    virtual float calculate_intensity(vec3d normal, mat4& view_mat) = 0;

protected:
    Color color;
};
