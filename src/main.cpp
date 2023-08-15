#define SDL_MAIN_HANDLED

#include <iostream>
#include <fpm/fixed.hpp>

#include "Window/Window.hpp"
#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "Mesh/Mesh.hpp"
#include "Texture/Texture.hpp"
#include "utils/Color/Color.hpp"
#include "utils/to_radians.hpp"

int main()
{
    // Initialize needed classes
    const vec2i dimensions = {512, 512};
    Window window({512, 512}, dimensions);
    Camera camera({-2, 0, -2}, {0, -to_radians(45), 0}, to_radians(60),
                  dimensions.y / dimensions.x, 10, 0.1);
    Scene scene(&window, &camera);

    // load the texture and mesh, position it in the world space
    Texture texture("./res/textures/half_life_box.ppm");
    Mesh mesh = Mesh("./res/models/cube_texture_test.obj", &texture);
    mesh.add_position({0, 0, 0}, {0, to_radians(0), 0});

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
