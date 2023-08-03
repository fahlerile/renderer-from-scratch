#include "Camera.hpp"
#include "utils/vec/vec3.hpp"
#include "utils/mat/mat.hpp"

Camera::Camera(vec3d position, double fov)
{
    this->position = position;
    this->fov = fov;
    this->update_matrices();
}

void Camera::update_matrices()
{
    this->view_mat = mat4(1.0);
    // negating here because I am basically translating
    // the whole world rather than only my camera
    this->view_mat = this->view_mat.translate(-this->position);

    this->projection_mat = mat4(1.0);
}

mat4 Camera::get_view_mat()
{
    return this->view_mat;
}

mat4 Camera::get_projection_mat()
{
    return this->projection_mat;
}
