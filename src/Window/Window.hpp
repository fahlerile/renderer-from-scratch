#pragma once
#include <SDL2/SDL.h>
#include "Color/Color.hpp"
#include "utils/vec.hpp"

class Window
{
public:
    Window(vec2i actual_dimensions, vec2i logical_dimensions);
    ~Window();

    void poll_events();
    void clear(Color color);
    void draw_pixel(vec2i position, Color color);
    void swap_buffers();

    bool is_running();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool running = true;
};