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

        // -1 because .obj is 1-indexed
        vec3d v0 = head.vertex(face[0] - 1);
        vec3d v1 = head.vertex(face[1] - 1);
        vec3d v2 = head.vertex(face[2] - 1);

        // cast to needed type and discard z coordinate
        vec2fix24_8 p0 = {fpm::fixed_24_8(v0.x),
                          fpm::fixed_24_8(v0.y)};
        vec2fix24_8 p1 = {fpm::fixed_24_8(v1.x),
                          fpm::fixed_24_8(v1.y)};
        vec2fix24_8 p2 = {fpm::fixed_24_8(v2.x),
                          fpm::fixed_24_8(v2.y)};

        // scale coordinates
        p0.x = (p0.x + 1) * dimensions.x / 2;
        p0.y = (p0.y + 1) * dimensions.y / 2;
        p1.x = (p1.x + 1) * dimensions.x / 2;
        p1.y = (p1.y + 1) * dimensions.y / 2;
        p2.x = (p2.x + 1) * dimensions.x / 2;
        p2.y = (p2.y + 1) * dimensions.y / 2;

        // transform coordinates to see not a flipped image
        p0 = {p0.x, -p0.y};
        p0.y += dimensions.y - 1;
        p1 = {p1.x, -p1.y};
        p1.y += dimensions.y - 1;
        p2 = {p2.x, -p2.y};
        p2.y += dimensions.y - 1;

        window.triangle(
            p2, p1, p0,
            {(unsigned char) distr(gen), (unsigned char) distr(gen), (unsigned char) distr(gen)},
            {(unsigned char) distr(gen), (unsigned char) distr(gen), (unsigned char) distr(gen)},
            {(unsigned char) distr(gen), (unsigned char) distr(gen), (unsigned char) distr(gen)}
        );
    }

    while (window.is_running())
    {
        this_frame_time = SDL_GetTicks();
        delta_time = this_frame_time - prev_frame_time;

        std::cout << "Frametime: " << delta_time << std::endl;

        window.poll_events();
        // window.clear({0, 0, 0});

        window.swap_buffers();

        prev_frame_time = this_frame_time;
    }

    return 0;
}
