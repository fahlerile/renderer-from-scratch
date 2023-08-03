#pragma once
#include <vector>
#include "Model/Model.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"

class Renderer
{
public:
    Renderer(Window* window, vec3d camera_pos, double camera_fov,
             double aspect_ratio, double zfar, double znear);

    void add_model(Model model);
    void render();

private:
    std::vector<Model> models;
    Camera camera;
    Window *window;
};