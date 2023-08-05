#include "DirectionalLight.hpp"
#include "utils/Color/Color.hpp"
#include "utils/vec/vec3.hpp"
#include "utils/mat/mat.hpp"

DirectionalLight::DirectionalLight(Color color, vec3d direction) : Light(color)
{
    this->direction = direction;
}

float DirectionalLight::calculate_intensity(vec3d normal, mat4& view_mat)
{
    // rotate direction according to view matrix
    vec4d rotated_direction = view_mat * (vec4d) {this->direction.x, direction.y, direction.z, 1};
    return normal.dot_product({rotated_direction.x, rotated_direction.y, rotated_direction.z});
}

vec3d DirectionalLight::get_direction()
{
    return this->direction;
}
