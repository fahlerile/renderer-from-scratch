#pragma once
#include <vector>
#include "Mesh/Mesh.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"

class Scene
{
public:
    Scene(Window* window, Camera* camera);

    void add_mesh(Mesh* mesh);
    void render();

private:
    Window *window;
    Camera* camera;

    std::vector<Mesh*> meshes;
};