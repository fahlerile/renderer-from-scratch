#include <iostream>
#include <SDL2/SDL.h>
#include <fpm/fixed.hpp>
#include "Color/Color.hpp"
#include "utils/vec/vec.hpp"
#include "utils/constants.hpp"
#include "Window.hpp"

Window::Window(vec2i actual_dimensions, vec2i logical_dimensions)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Failed to initialize SDL2 library: " << SDL_GetError() << std::endl;
	    std::exit(SDL2_INIT_FAIL_EXIT_CODE);
    }
    SDL_CreateWindowAndRenderer(actual_dimensions.x, actual_dimensions.y, 0, &(this->window), &(this->renderer));
    SDL_RenderSetLogicalSize(this->renderer, logical_dimensions.x, logical_dimensions.y);
}

Window::~Window()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void Window::poll_events()
{
    while (SDL_PollEvent(&(this->event)))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            this->running = false;

        if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
                this->rotate = !(this->rotate);
            else if (event.key.keysym.sym == SDLK_q)
                this->draw_first = !(this->draw_first);
            else if (event.key.keysym.sym == SDLK_w)
                this->draw_second = !(this->draw_second);
        }
    }
}

void Window::clear(Color color)
{
    SDL_SetRenderDrawColor(this->renderer, color.red, color.green, color.blue, 0);
    SDL_RenderClear(this->renderer);
}

void Window::draw_pixel(vec2i position, Color color)
{
    SDL_SetRenderDrawColor(this->renderer, color.red, color.green, color.blue, 0);
    SDL_RenderDrawPoint(this->renderer, position.x, position.y);
}

void Window::swap_buffers()
{
    SDL_RenderPresent(this->renderer);
}

bool Window::is_running()
{
    return this->running;
}

// Bresenham's algorithm
// See https://www.ics.uci.edu/~gopi/CS112/web/handouts/OldFiles/Bresenham.pdf
void Window::line(vec2i pos0, vec2i pos1, Color color)
{
    int x0 = pos0.x;
    int y0 = pos0.y;
    int x1 = pos1.x;
    int y1 = pos1.y;

    bool steep = false;
    if (abs(x1-x0) < abs(y1-y0))  // if slope > 1, transpose
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1)  // need to render from left to right
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int step = (y0 > y1) ? -1 : 1;

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    int x = x0;
    int y = y0;

    int D = 2 * dy - dx;

    if (!steep)
        this->draw_pixel({x, y}, color);
    else  // if steep, de-transpose
        this->draw_pixel({y, x}, color);

    while (x < x1)
    {
        x++;
        if (D < 0)
            D += (2 * dy);
        else
        {
            y += step;
            D += (2 * (dy - dx));
        }
        if (!steep)
            this->draw_pixel({x, y}, color);
        else  // if steep, de-transpose
            this->draw_pixel({y, x}, color);
    }
}

// Vertices are assumed to be in counter-clockwise direction
// Top-left rasterization rule
// https://www.youtube.com/watch?v=k5wtuKWmV48
void Window::triangle(vec2fix24_8 v0, vec2fix24_8 v1, vec2fix24_8 v2,
                      Color c0,       Color c1,       Color c2)
{
    using vec2fix = vec2fix24_8;
    using fixed = fpm::fixed_24_8;

    // a, b, p is points
    auto edge_function([](vec2fix a, vec2fix b, vec2fix p)
    {
        // get vectors that represent sides AB and AP
        vec2fix ab = {b.x - a.x, b.y - a.y};
        vec2fix ap = {p.x - a.x, p.y - a.y};
        // get z-component of vec3(ab, 0) x vec3(ap, 0)
        return ab.x * ap.y - ab.y * ap.x;
    });

    // helper function for rasterization rules implementation
    // `start` and `end` are points
    auto is_top_left([](vec2fix start, vec2fix end)
    {
        vec2fix edge = {end.x - start.x, end.y - start.y};

        bool is_top = (edge.y == (fixed) {0} && edge.x < (fixed) {0});
        bool is_left = (edge.y < (fixed) {0});

        return is_top || is_left;
    });

    // determine the bounding box
    // TODO: fix floor ceil errors
    int xmin = fpm::floor(std::min(std::min(v0.x, v1.x), v2.x));
    int ymin = fpm::floor(std::min(std::min(v0.y, v1.y), v2.y));
    int xmax = fpm::ceil(std::max(std::max(v0.x, v1.x), v2.x));
    int ymax = fpm::ceil(std::max(std::max(v0.y, v1.y), v2.y));

    // calculate edge biases  TODO: CHANGE 1 TO SMALLEST POSITIVE NUMBER IN FIXED_24_8 TYPE
    fixed bias0 { (is_top_left(v0, v1)) ? 0 : 1 };
    fixed bias1 { (is_top_left(v1, v2)) ? 0 : 1 };
    fixed bias2 { (is_top_left(v2, v0)) ? 0 : 1 };

    // calculate the area of the parallelogram formed by vectors v0v1 and v0v2
    fixed area = edge_function(v0, v1, v2);

    // calculate initlial value of edge function
    // for each edge and first point (incremental computation)
    vec2fix p0 = {xmin + 0.5, ymin + 0.5};
    fixed w0_begin = edge_function(v0, v1, p0) + bias0;
    fixed w1_begin = edge_function(v1, v2, p0) + bias1;
    fixed w2_begin = edge_function(v2, v0, p0) + bias2;

    // set delta w constants
    const fixed dx_w0 = v0.y - v1.y;
    const fixed dy_w0 = v1.x - v0.x;
    const fixed dx_w1 = v1.y - v2.y;
    const fixed dy_w1 = v2.x - v1.x;
    const fixed dx_w2 = v2.y - v0.y;
    const fixed dy_w2 = v0.x - v2.x;

    // traverse over each pixel in bounding box
    for (int y = ymin; y < ymax; y++)
    {
        // set w values to the beginning of this row
        fixed w0 = w0_begin;
        fixed w1 = w1_begin;
        fixed w2 = w2_begin;

        for (int x = xmin; x < xmax; x++)
        {
            bool pixel_inside_triangle = (w0 <= {0}) &&
                                         (w1 <= {0}) &&
                                         (w2 <= {0});

            if (pixel_inside_triangle)
            {
                // calculate barycentric coordinates for this pixel
                // (alpha - v0, beta - v1, gamma - v2)
                fixed gamma = w0 / area;  // <= this is gamma because it shows
                fixed alpha = w1 / area;  // how much I "pull" to the v2
                fixed beta = w2 / area;   // in percents, etc. for others
                Color color = (c0 * alpha) + (c1 * beta) + (c2 * gamma);

                this->draw_pixel({x, y}, color);
            }

            // change w value for the next pixel in this row
            w0 = w0 + dx_w0;
            w1 = w1 + dx_w1;
            w2 = w2 + dx_w2;
        }

        // update w value to the next
        w0_begin += dy_w0;
        w1_begin += dy_w1;
        w2_begin += dy_w2;
    }
}