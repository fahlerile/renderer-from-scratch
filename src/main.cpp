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

    Window window({800, 800}, dimensions);

    unsigned int this_frame_time = 0;
    unsigned int delta_time = 0;
    unsigned int prev_frame_time = 0;

    Color white = Color(255, 255, 255);
    Model head = Model("./res/models/african_head.obj");

    // initialize random color generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 255);

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

        // 2 edge vectors to calculate normal vector
        vec3d edge[2] = {
            {  // TODO: USE OVERLOADED OPERATORS TO DO THIS
                v[1].x - v[0].x,
                v[1].y - v[0].y,
                v[1].z - v[0].z,
            },
            {
                v[2].x - v[0].x,
                v[2].y - v[0].y,
                v[2].z - v[0].z,
            }
        };
        vec3d normal = edge[1].cross_product(edge[0]);
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

        vec3d light = {0, 0, 1};
        float intensity = normal.dot_product(light);

        if (intensity > 0)
        {
            Color color = {255, 255, 255};
            color = color * intensity;
            window.triangle(p[2], p[1], p[0], color);
        }
    }

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
