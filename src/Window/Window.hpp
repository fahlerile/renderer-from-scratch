#pragma once
#include <SDL2/SDL.h>
#include "Color/Color.hpp"
#include "utils/vec/vec.hpp"

class Window
{
public:
    Window(vec2i actual_dimensions, vec2i logical_dimensions);
    ~Window();

    void poll_events();

    void clear(Color color);
    void swap_buffers();

    void draw_pixel(vec2i position, Color color);
    void line(vec2i pos0, vec2i pos1, Color color);
    void triangle(vec2f v0, vec2f v1, vec2f v2,
                  Color c0, Color c1, Color c2);

    bool is_running();

    // temporary
    bool rotate = true;
    bool draw_first = true;
    bool draw_second = true;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool keyboard[322];
    bool running = true;
};