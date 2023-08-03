#define SDL_MAIN_HANDLED
#include <iostream>
#include <fpm/fixed.hpp>
#include <fpm/ios.hpp>
#include "Window/Window.hpp"
#include "Model/Model.hpp"
#include "Color/Color.hpp"

#include <random>

int main()
{
    const vec2i dimensions = {512, 512};
    Window window({1024, 1024}, dimensions);

    Model head = Model("./res/models/african_head.obj");
    head.add_position({0, 0, 0});
    // head.render();

    unsigned int this_frame_time = 0;
    unsigned int delta_time = 0;
    unsigned int prev_frame_time = 0;

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
