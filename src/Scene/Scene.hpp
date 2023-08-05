#pragma once
#include <vector>
#include "Mesh/Mesh.hpp"
#include "Light/Light.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"

class Scene
{
public:
    Scene(Window* window, Camera* camera);

    void add_mesh(Mesh* mesh);
    void add_light(Light* light);
    void render();

private:
    Window *window;
    Camera* camera;

    std::vector<Mesh*> meshes;
    std::vector<Light*> lights;
};