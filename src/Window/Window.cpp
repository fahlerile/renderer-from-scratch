#include <iostream>
#include <SDL2/SDL.h>
#include "Color/Color.hpp"
#include "utils/vec.hpp"
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
    if (SDL_PollEvent(&(this->event)) && this->event.type == SDL_QUIT)
        this->running = false;
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
void Window::triangle(vec2i v0, vec2i v1, vec2i v2,
                      Color c0, Color c1, Color c2)
{
    // a, b, p is points
    auto edge_function([](vec2i a, vec2i b, vec2i p)
    {
        // get vectors that represent sides AB and AP
        vec2i ab = {b.x - a.x, b.y - a.y};
        vec2i ap = {p.x - a.x, p.y - a.y};
        // get z-component of vec3(ab, 0) X vec3(ap, 0)
        return ab.x * ap.y - ab.y * ap.x;
    });

    // helper function for rasterization rules implementation
    // `start` and `end` are points
    auto is_top_left([](vec2i start, vec2i end)
    {
        vec2i edge = {end.x - start.x, end.y - start.y};

        bool is_top = (edge.y == 0 && edge.x < 0);
        bool is_left = (edge.y < 0);

        return is_top || is_left;
    });

    // determine the bounding box
    int xmin = std::min(std::min(v0.x, v1.x), v2.x);
    int ymin = std::min(std::min(v0.y, v1.y), v2.y);
    int xmax = std::max(std::max(v0.x, v1.x), v2.x);
    int ymax = std::max(std::max(v0.y, v1.y), v2.y);

    // calculate edge biases
    int bias0 = (is_top_left(v0, v1)) ? 0 : 1;
    int bias1 = (is_top_left(v1, v2)) ? 0 : 1;
    int bias2 = (is_top_left(v2, v0)) ? 0 : 1;

    // calculate the area of the parallelogram formed by vectors v0v1 and v0v2
    int area = edge_function(v0, v1, v2);

    // calculate initlial value of edge function
    // for each edge and first point (incremental computation)
    vec2i p0 = {xmin, ymin};
    int w0_begin = edge_function(v0, v1, p0) + bias0;
    int w1_begin = edge_function(v1, v2, p0) + bias1;
    int w2_begin = edge_function(v2, v0, p0) + bias2;

    // set delta w constants
    const int dx_w0 = v0.y - v1.y;
    const int dy_w0 = v1.x - v0.x;
    const int dx_w1 = v1.y - v2.y;
    const int dy_w1 = v2.x - v1.x;
    const int dx_w2 = v2.y - v0.y;
    const int dy_w2 = v0.x - v2.x;

    // traverse over each pixel in bounding box
    for (int y = ymin; y < ymax; y++)
    {
        // set w values to the beginning of this row
        int w0 = w0_begin;
        int w1 = w1_begin;
        int w2 = w2_begin;

        for (int x = xmin; x < xmax; x++)
        {
            bool pixel_inside_triangle = (w0 <= 0) &&
                                         (w1 <= 0) &&
                                         (w2 <= 0);

            if (pixel_inside_triangle)
            {
                // calculate barycentric coordinates for this pixel
                // (alpha - v0, beta - v1, gamma - v2)
                float gamma = (float) w0 / area;  // this is gamma because it shows
                float alpha = (float) w1 / area;  // how much I "pull" to the v2
                float beta = (float) w2 / area;   // in percents, etc. for others
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