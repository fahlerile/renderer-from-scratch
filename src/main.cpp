#define SDL_MAIN_HANDLED
#include "Window/Window.hpp"
#include "Color/Color.hpp"

#include <iostream>  // debug

int main()
{
    vec2i dimensions = {100, 100};
    Window window({800, 800}, dimensions);

    vec2i vertices[3] = {
        {19, 19},
        {19, 79},
        {79, 79}
    };

    Color colors[3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255}
    };

    vec2i center = {dimensions.x / 2, dimensions.y / 2};

    while (window.is_running())
    {
        window.poll_events();
        window.clear({0, 0, 0});

        float angle_rad = SDL_GetTicks() / 100 * 0.1;

        vec2i v0 = vertices[0].rotate(angle_rad, center);
        vec2i v1 = vertices[1].rotate(angle_rad, center);
        vec2i v2 = vertices[2].rotate(angle_rad, center);

        window.triangle(v0, v1, v2, colors[0], colors[1], colors[2]);

        window.swap_buffers();
    }

    return 0;
}
