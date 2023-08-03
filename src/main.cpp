#define SDL_MAIN_HANDLED

#include <iostream>
#include <fpm/fixed.hpp>

#include "Window/Window.hpp"
#include "Renderer/Renderer.hpp"
#include "Model/Model.hpp"
#include "Color/Color.hpp"

int main()
{
    const vec2i dimensions = {512, 512};
    Window window({1024, 1024}, dimensions);
    Renderer renderer(&window, {0, 0, 0}, 45.0);

    Model head = Model("./res/models/african_head.obj");
    head.add_position({0, 0, 0});
    renderer.add_model(head);

    renderer.render();

    std::cout << "Rendered!" << std::endl;

    unsigned int this_frame_time = 0;
    unsigned int delta_time = 0;
    unsigned int prev_frame_time = 0;

    // window.triangle(
    //     {
    //         (vec4d) {0, 0.5, 0, 1},
    //         (vec4d) {-0.5, -0.5, 0, 1},
    //         (vec4d) {0.5, -0.5, 0, 1}
    //     },
    //     {255, 255, 255}
    // );

    while (window.is_running())
    {
        this_frame_time = SDL_GetTicks();
        delta_time = this_frame_time - prev_frame_time;

        // std::cout << "Frametime: " << delta_time << std::endl;

        window.poll_events();
        // window.clear({0, 0, 0});

        window.swap_buffers();

        prev_frame_time = this_frame_time;
    }

    return 0;
}
