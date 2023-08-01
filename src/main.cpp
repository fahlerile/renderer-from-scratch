#include <iostream>
#define SDL_MAIN_HANDLED
#include "Window/Window.hpp"
#include "Color/Color.hpp"

int main()
{
    vec2i dimensions = {200, 200};
    vec2f center = {(float) dimensions.x / 2, (float) dimensions.y / 2};

    Window window({800, 800}, dimensions);

    vec2f vertices[4] = {
        {99, 29},
        {49, 99},
        {149, 99},
        {99, 149}
    };

    Color colors[4] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 255}
    };

    unsigned int this_frame_time = 0;
    unsigned int delta_time = 0;
    unsigned int prev_frame_time = 0;

    while (window.is_running())
    {
        this_frame_time = SDL_GetTicks();
        delta_time = this_frame_time - prev_frame_time;

        std::cout << "Frametime: " << delta_time << std::endl;

        window.poll_events();
        window.clear({0, 0, 0});

        float angle = SDL_GetTicks() / 10 * 0.01;
        vec2fix24_8 v0 = vertices[0].rotate(angle, center);
        vec2fix24_8 v1 = vertices[1].rotate(angle, center);
        vec2fix24_8 v2 = vertices[2].rotate(angle, center);
        vec2fix24_8 v3 = vertices[3].rotate(angle, center);

        window.triangle(v0, v1, v2, colors[0], colors[1], colors[2]);
        window.triangle(v1, v3, v2, colors[1], colors[3], colors[2]);

        window.swap_buffers();

        prev_frame_time = this_frame_time;
    }

    return 0;
}
