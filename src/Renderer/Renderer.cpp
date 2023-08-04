#include "Model/Model.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"
#include "Renderer.hpp"

Renderer::Renderer(Window* window, vec3d camera_pos, vec3d camera_rot_angles,
                   double camera_fov, double aspect_ratio, double zfar, double znear) :
                   camera(camera_pos, camera_rot_angles, camera_fov, aspect_ratio, zfar, znear)
{
    this->window = window;
}

void Renderer::add_model(Model model)
{
    this->models.push_back(model);
}

void Renderer::render()
{
    mat4 view_mat = this->camera.get_view_mat();
    mat4 projection_mat = this->camera.get_projection_mat();
    vec4d front_vec = this->camera.get_front_vec();

    for (auto model : this->models)
    {
        model.render(window, view_mat, projection_mat, front_vec);
    }
}