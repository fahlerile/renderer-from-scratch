#define SDL_MAIN_HANDLED
#include "Window/Window.hpp"
#include "Color/Color.hpp"

int main()
{
    Window window({800, 800}, {100, 100});

    while (window.is_running())
    {
        window.poll_events();
        window.clear({0, 0, 0});
        for (int i = 0; i < 50; i++)
            window.draw_pixel({i, i}, {255, 255, 255});
        window.swap_buffers();
    }

    return 0;
}
