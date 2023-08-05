#include "DirectionalLight.hpp"
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"

DirectionalLight::DirectionalLight(Color color, vec3d direction) : Light(color)
{
    this->direction = direction;
}

float DirectionalLight::calculate_intensity(vec3d normal)
{
    return normal.dot_product(this->direction);
}

vec3d DirectionalLight::get_direction()
{
    return this->direction;
}
