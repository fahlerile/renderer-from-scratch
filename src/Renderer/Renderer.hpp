#pragma once
#include <vector>
#include "Model/Model.hpp"
#include "Camera/Camera.hpp"
#include "Window/Window.hpp"

class Renderer
{
public:
    Renderer(Window* window, Camera* camera);

    void add_model(Model model);
    void render();

private:
    Window *window;
    Camera* camera;
    std::vector<Model> models;
};