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
    void line(std::vector<vec4d> p_ndc, std::vector<Color> c);
    void line(std::vector<vec4d> p_ndc, Color c);
    void triangle(std::vector<vec4d> v_dnc, std::vector<Color> c);
    void triangle(std::vector<vec4d> v_dnc, Color c);

    bool is_running();

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Event sdl_event;

    std::vector<std::vector<double>> z_buffer;  // more positive - further away

    vec2i dimensions;
    bool running = true;

    vec2i ndc_to_pixel(vec2d v_dnc);
    void clear_z_buffer();
};