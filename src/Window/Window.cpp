#include <iostream>
#include <SDL2/SDL.h>
#include <fpm/fixed.hpp>
#include <fpm/math.hpp>
#include <fpm/ios.hpp>
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

        // if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {}
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
// Uses top-left rasterization rule
// https://www.youtube.com/watch?v=k5wtuKWmV48
void Window::triangle(std::vector<vec2fix24_8> v, std::vector<Color> c)
{
    using vec2fix = vec2fix24_8;
    using fixed = fpm::fixed_24_8;

    const int decimal_bits = 8;
    const fixed smallest_positive = fixed::from_raw_value(1);

    // a, b, p is points
    auto edge_function([](vec2fix a, vec2fix b, vec2fix p)
    {
        // get vectors that represent sides AB and AP
        vec2fix ab = b - a;
        vec2fix ap = p - a;
        // get z-component of vec3(ab, 0) x vec3(ap, 0)
        return ab.cross_product(ap).z;
    });

    // helper function for rasterization rules implementation
    // `start` and `end` are points
    auto is_top_left([](vec2fix start, vec2fix end)
    {
        vec2fix edge = end - start;

        bool is_top = (edge.y == (fixed) {0} && edge.x < (fixed) {0});
        bool is_left = (edge.y < (fixed) {0});

        return is_top || is_left;
    });

    // calculate the area of the parallelogram formed by vectors v[0]v[1] and v[0]v[2]
    // needed for barycentic coordinate interpolation
    const fixed area = edge_function(v[0], v[1], v[2]);

    // determine the bounding box (min and max values of x and y)
    const vec2i min = {
        (int) fpm::floor(std::min(std::min(v[0].x, v[1].x), v[2].x)),
        (int) fpm::floor(std::min(std::min(v[0].y, v[1].y), v[2].y))
    };
    const vec2i max = {
        (int) fpm::ceil(std::max(std::max(v[0].x, v[1].x), v[2].x)),
        (int) fpm::ceil(std::max(std::max(v[0].y, v[1].y), v[2].y))
    };

    // calculate edge biases
    const fixed bias[3] = {
        (is_top_left(v[0], v[1])) ? fixed(0) : smallest_positive,
        (is_top_left(v[1], v[2])) ? fixed(0) : smallest_positive,
        (is_top_left(v[2], v[0])) ? fixed(0) : smallest_positive,
    };

    // calculate initlial value of edge function
    // for each edge and first point (incremental computation)
    vec2f p0 = {min.x + 0.5f, min.y + 0.5f};
    fixed w_initial[3] = {
        edge_function(v[0], v[1], p0) + bias[0],
        edge_function(v[1], v[2], p0) + bias[1],
        edge_function(v[2], v[0], p0) + bias[2]
    };

    // set delta w constants
    const vec2fix24_8 dw[3] = {
        {
            v[0].y - v[1].y,
            v[1].x - v[0].x
        },
        {
            v[1].y - v[2].y,
            v[2].x - v[1].x
        },
        {
            v[2].y - v[0].y,
            v[0].x - v[2].x
        }
    };

    // traverse over each pixel in bounding box
    for (int y = min.y; y < max.y; y++)
    {
        // set w values to the beginning of this row
        fixed w[3] = {
            w_initial[0],
            w_initial[1],
            w_initial[2]
        };

        for (int x = min.x; x < max.x; x++)
        {
            bool pixel_inside_triangle = (w[0] <= fixed(0)) &&
                                         (w[1] <= fixed(0)) &&
                                         (w[2] <= fixed(0));

            if (pixel_inside_triangle)
            {
                // calculate barycentric coordinates for this pixel
                // (alpha - v[0], beta - v[1], gamma - v[2])
                double gamma = (double) (w[0] / area);  // <= this is gamma because it shows
                double alpha = (double) (w[1] / area);  // how much I "pull" to the v[2]
                double beta = (double) (w[2] / area);   // in percents, etc. for others
                Color color = (c[0] * alpha) + (c[1] * beta) + (c[2] * gamma);

                this->draw_pixel({x, y}, color);
            }

            // change w value for the next pixel in this row
            w[0] = w[0] + dw[0].x;
            w[1] = w[1] + dw[1].x;
            w[2] = w[2] + dw[2].x;
        }

        // update w value to the next
        w_initial[0] += dw[0].y;
        w_initial[1] += dw[1].y;
        w_initial[2] += dw[2].y;
    }
}

void Window::triangle(std::vector<vec2fix24_8> v, Color c)
{
    this->triangle({v[0], v[1], v[2]}, {c, c, c});
}