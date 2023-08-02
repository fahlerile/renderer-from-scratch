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

    for (int i = 0; i < head.n_faces(); i++)
    {
        vec3i face = head.face(i);

        // vertices, world space
        // -1 because .obj is 1-indexed
        vec3d v[3] = {
            head.vertex(face[0] - 1),
            head.vertex(face[1] - 1),
            head.vertex(face[2] - 1)
        };

        // calculate normal vector for lighting
        vec3d normal = (v[2]-v[0]).cross_product(v[1]-v[0]);
        normal = normal.normalize();

        // points on the screen, screen space
        // cast to needed type, discard z coordinate and scale coordinates
        vec2fix24_8 p[3] = {
            {
                (fpm::fixed_24_8(v[0].x) + 1) * dimensions.x / 2,
                (fpm::fixed_24_8(v[0].y) + 1) * dimensions.y / 2
            },
            {
                (fpm::fixed_24_8(v[1].x) + 1) * dimensions.x / 2,
                (fpm::fixed_24_8(v[1].y) + 1) * dimensions.y / 2
            },
            {
                (fpm::fixed_24_8(v[2].x) + 1) * dimensions.x / 2,
                (fpm::fixed_24_8(v[2].y) + 1) * dimensions.y / 2
            }
        };

        // transform coordinates to see not a flipped image
        p[0] = {p[0].x, -p[0].y};
        p[0].y += dimensions.y - 1;
        p[1] = {p[1].x, -p[1].y};
        p[1].y += dimensions.y - 1;
        p[2] = {p[2].x, -p[2].y};
        p[2].y += dimensions.y - 1;

        // calculate light intensity
        vec3d light = {0, 0, 1};
        Color light_color = {255, 255, 255};
        float intensity = normal.dot_product(light);

        // backface culling (rendering only faces with light)
        if (intensity > 0)
        {
            Color color = light_color * intensity;
            window.triangle({p[2], p[1], p[0]}, color);
        }
    }

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
