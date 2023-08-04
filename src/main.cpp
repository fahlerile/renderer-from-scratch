#define SDL_MAIN_HANDLED

#include <iostream>
#include <fpm/fixed.hpp>

#include "Window/Window.hpp"
#include "Camera/Camera.hpp"
#include "Renderer/Renderer.hpp"
#include "Model/Model.hpp"
#include "Color/Color.hpp"
#include "utils/to_radians.hpp"

int main()
{
    const vec2i dimensions = {512, 512};

    // Initialize needed classes
    Window window({512, 512}, dimensions);
    Camera camera({0, 0, 3}, {0, to_radians(180), 0}, to_radians(45),
                  dimensions.y / dimensions.x, 10, 0.1);
    Renderer renderer(&window, &camera);

    // load the model
    Model head = Model("./res/models/african_head.obj");
    head.add_position({0, 0, 0}, {0, to_radians(0), 0});

    renderer.add_model(head);

    window.clear({128, 204, 204});
    renderer.render();

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
