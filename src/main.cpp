#define SDL_MAIN_HANDLED

#include <iostream>
#include <fpm/fixed.hpp>

#include "Window/Window.hpp"
#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "Mesh/Mesh.hpp"
#include "Light/DirectionalLight.hpp"
#include "utils/Color/Color.hpp"
#include "utils/to_radians.hpp"

int main()
{
    const vec2i dimensions = {512, 512};

    // Initialize needed classes
    Window window({512, 512}, dimensions);
    Camera camera({0, 0, -2}, {0, to_radians(0), 0}, to_radians(45),
                  dimensions.y / dimensions.x, 10, 0.1);
    Scene scene(&window, &camera);

    // load the mesh, position it in the world space
    Mesh mesh = Mesh("./res/models/african_head.obj");
    mesh.add_position({0, 0, 0}, {0, to_radians(90), 0});

    // create a light object
    DirectionalLight light({255, 255, 255}, (vec3d) {0, 0, 1}.normalize());

    // add mesh and light to scene
    scene.add_mesh(&mesh);
    scene.add_light(&light);

    window.clear({128, 204, 204});
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
