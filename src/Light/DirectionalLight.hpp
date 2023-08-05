#pragma once
#include "Light/Light.hpp"
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"

// Directional light class. Has a direction, but it does not care about distance to an object.
class DirectionalLight : public Light
{
public:
    DirectionalLight(Color color, vec3d direction);
    float calculate_intensity(vec3d normal, mat4& view_mat);
    vec3d get_direction();

private:
    vec3d direction;
};
