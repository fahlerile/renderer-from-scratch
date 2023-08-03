#pragma once
#include "utils/vec/vec3.hpp"
#include "utils/mat/mat.hpp"

class Camera
{
public:
    Camera(vec3d position, double fov, double aspect_ratio, double zfar, double znear);

    void update_matrices();

    mat4 get_view_mat();
    mat4 get_projection_mat();

private:
    vec3d position;
    double fov;
    double aspect_ratio;
    double zfar;
    double znear;

    mat4 view_mat;
    mat4 projection_mat;
};
