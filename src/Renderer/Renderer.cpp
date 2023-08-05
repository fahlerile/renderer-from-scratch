#include "Mesh/Mesh.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"
#include "Renderer.hpp"

Renderer::Renderer(Window* window, Camera* camera)
{
    this->window = window;
    this->camera = camera;
}

void Renderer::add_mesh(Mesh mesh)
{
    this->meshes.push_back(mesh);
}

void Renderer::render()
{
    mat4 view_mat = this->camera->get_view_mat();
    mat4 projection_mat = this->camera->get_projection_mat();
    vec4d front_vec = this->camera->get_front_vec();

    for (auto mesh : this->meshes)
    {
        mesh.render(window, view_mat, projection_mat, front_vec);
    }
}