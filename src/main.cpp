#define SDL_MAIN_HANDLED
#include "Window/Window.hpp"
#include "Color/Color.hpp"

bool rotate = true;
bool draw_first = true;
bool draw_second = true;

int main()
{
    vec2i dimensions = {100, 100};
    Window window({800, 800}, dimensions);

    vec2f vertices[4] = {
        {19, 19},
        {19, 79},
        {79, 79},
        {79, 19}
    };

    Color colors[3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255}
    };

    vec2f center = {(float) dimensions.x / 2, (float) dimensions.y / 2};
    float angle_save = 0.0f;

    while (window.is_running())
    {
        window.poll_events();
        window.clear({0, 0, 0});

        vec2f v0;
        vec2f v1;
        vec2f v2;
        vec2f v3;

        if (window.rotate)
        {
            float angle_rad = SDL_GetTicks() / 10 * 0.01;
            v0 = vertices[0].rotate(angle_rad, center);
            v1 = vertices[1].rotate(angle_rad, center);
            v2 = vertices[2].rotate(angle_rad, center);
            v3 = vertices[3].rotate(angle_rad, center);
            angle_save = angle_rad;
        }
        else
        {
            v0 = vertices[0].rotate(angle_save, center);
            v1 = vertices[1].rotate(angle_save, center);
            v2 = vertices[2].rotate(angle_save, center);
            v3 = vertices[3].rotate(angle_save, center);
        }

        if (window.draw_first)
            window.triangle(v0, v1, v2, colors[0], colors[1], colors[2]);
        if (window.draw_second)
            window.triangle(v3, v0, v2, {255, 255, 255}, {255, 255, 255}, {255, 255, 255});

        window.swap_buffers();
    }

    return 0;
}
