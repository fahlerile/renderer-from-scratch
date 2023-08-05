#include "Light.hpp"
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"

Light::Light(Color color)
{
    this->color = color;
}

Color Light::get_color()
{
    return this->color;
}
