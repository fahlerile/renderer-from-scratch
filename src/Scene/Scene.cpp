#include "Mesh/Mesh.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"
#include "Scene.hpp"

Scene::Scene(Window* window, Camera* camera)
{
    this->window = window;
    this->camera = camera;
}

void Scene::add_mesh(Mesh* mesh)
{
    this->meshes.push_back(mesh);
}

void Scene::render()
{
    mat4 view_mat = this->camera->get_view_mat();
    mat4 projection_mat = this->camera->get_projection_mat();

    for (auto mesh : this->meshes)
    {
        mesh->render(window, view_mat, projection_mat);
    }
}