#pragma once
#include "utils/vec/vec3.hpp"
#include "utils/mat/mat.hpp"

class Camera
{
public:
    Camera(vec3d position, double fov);

    void update_matrices();

    mat4 get_view_mat();
    mat4 get_projection_mat();

private:
    vec3d position;
    double fov;

    mat4 view_mat;
    mat4 projection_mat;
};