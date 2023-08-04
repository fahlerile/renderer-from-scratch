#include "Camera.hpp"
#include "utils/vec/vec3.hpp"
#include "utils/mat/mat.hpp"

// `aspect_ratio` - screen_height/screen_width
Camera::Camera(vec3d position, vec3d rot_angles, double fov,
               double aspect_ratio, double zfar, double znear)
{
    this->position = position;
    this->rot_angles = rot_angles;
    this->fov = fov;
    this->aspect_ratio = aspect_ratio;
    this->zfar = zfar;
    this->znear = znear;
    this->update_matrices();
}

void Camera::update_matrices()
{
    this->view_mat = mat4(1.0);
    // negating here because I am basically translating (and rotating)
    // the whole world rather than only my camera
    this->view_mat = this->view_mat.translate(-this->position);
    this->view_mat = this->view_mat.rotate(-this->rot_angles);

    // https://www.youtube.com/watch?v=EqNcqBdrNyI
    this->projection_mat = mat4(1.0);
    double alpha = 1 / std::tan(this->fov / 2);  // fov scaling factor
    double lambda = this->zfar / (this->zfar - this->znear);  // z normalizing factor
    this->projection_mat.data[0][0] = this->aspect_ratio * alpha;
    this->projection_mat.data[1][1] = alpha;
    this->projection_mat.data[2][2] = lambda;
    this->projection_mat.data[2][3] = -lambda * this->znear;
    this->projection_mat.data[3][2] = 1;
}

mat4 Camera::get_view_mat()
{
    return this->view_mat;
}

mat4 Camera::get_projection_mat()
{
    return this->projection_mat;
}
