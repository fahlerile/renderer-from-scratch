#define SDL_MAIN_HANDLED
#include "Window/Window.hpp"
#include "Color/Color.hpp"

int main()
{
    Window window({800, 800}, {100, 100});

    vec2i vertices[3] = {
        {49, 19},
        {19, 79},
        {79, 79}
    };

    Color colors[3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255}
    };

    while (window.is_running())
    {
        window.poll_events();
        window.clear({0, 0, 0});

        window.triangle(vertices[0], vertices[1], vertices[2],
                        colors[0],   colors[1],   colors[2]);

        window.swap_buffers();
    }

    return 0;
}
