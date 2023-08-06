#define SDL_MAIN_HANDLED

#include <iostream>
#include <fpm/fixed.hpp>

#include "Window/Window.hpp"
#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "Mesh/Mesh.hpp"
#include "utils/Color/Color.hpp"
#include "utils/to_radians.hpp"

int main()
{
    const vec2i dimensions = {1024, 1024};

    // Initialize needed classes
    Window window({1024, 1024}, dimensions);
    Camera camera({0, 0, -1}, {0, to_radians(0), 0}, to_radians(60),
                  dimensions.y / dimensions.x, 10, 0.1);
    Scene scene(&window, &camera);

    // load the mesh, position it in the world space
    Mesh mesh = Mesh("./res/models/african_head.obj");
    mesh.add_position({0, 0, 0}, {0, to_radians(180), 0});

    // add mesh to scene
    scene.add_mesh(&mesh);

    window.clear({0, 0, 0});
    scene.render();

    std::cout << "Rendered!" << std::endl;

    unsigned int this_frame_time = 0;
    unsigned int delta_time = 0;
    unsigned int prev_frame_time = 0;

    while (window.is_running())
    {
        this_frame_time = SDL_GetTicks();
        delta_time = this_frame_time - prev_frame_time;

        // std::cout << "Frametime: " << delta_time << std::endl;

        window.poll_events();
        window.swap_buffers();

        prev_frame_time = this_frame_time;
    }

    return 0;
}
