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
    void triangle(std::vector<vec4d> v_dnc, std::vector<Color> c);
    void triangle(std::vector<vec4d> v_dnc, Color c);

    vec2i dnc_to_pixel(vec2d v_dnc);

    bool is_running();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    vec2i dimensions;
    bool running = true;
};