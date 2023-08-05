#include "DirectionalLight.hpp"
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"

Light::Light(Color color)
{
    this->color = color;
}

float Light::calculate_intensity(vec3d normal)
{
    return 0;
}

Color Light::get_color()
{
    return this->color;
}

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
