#include <iostream>
#include <SDL2/SDL.h>
#include <fpm/fixed.hpp>
#include <fpm/math.hpp>
#include "Texture/Texture.hpp"
#include "utils/Color/Color.hpp"
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
    SDL_CreateWindowAndRenderer(actual_dimensions.x, actual_dimensions.y, 0,
                                &(this->sdl_window), &(this->sdl_renderer));
    SDL_RenderSetLogicalSize(this->sdl_renderer, logical_dimensions.x, logical_dimensions.y);

    this->dimensions = logical_dimensions;

    // Initialize z_buffer to all +1s (more positive - further away)
    for (int i = 0; i < logical_dimensions.y; i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < logical_dimensions.x; j++)
            temp.push_back(1);
        this->z_buffer.push_back(temp);
    }
}

Window::~Window()
{
    SDL_DestroyRenderer(this->sdl_renderer);
    SDL_DestroyWindow(this->sdl_window);
    SDL_Quit();
}

void Window::poll_events()
{
    while (SDL_PollEvent(&(this->sdl_event)))
    {
        if (sdl_event.type == SDL_QUIT || (sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.sym == SDLK_ESCAPE))
            this->running = false;

        // if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.repeat == 0) {}
    }
}

void Window::clear(Color color)
{
    SDL_SetRenderDrawColor(this->sdl_renderer, color.red, color.green, color.blue, 0);
    SDL_RenderClear(this->sdl_renderer);
}

void Window::draw_pixel(vec2i position, Color color)
{
    SDL_SetRenderDrawColor(this->sdl_renderer, color.red, color.green, color.blue, 0);
    SDL_RenderDrawPoint(this->sdl_renderer, position.x, position.y);
}

void Window::swap_buffers()
{
    SDL_RenderPresent(this->sdl_renderer);
    this->clear_z_buffer();
}

bool Window::is_running()
{
    return this->running;
}

// Bresenham's algorithm
// See https://www.ics.uci.edu/~gopi/CS112/web/handouts/OldFiles/Bresenham.pdf
void Window::line(std::vector<vec4d> p_ndc, std::vector<Color> c)
{
    // transform ndc to pixel coordinates
    // keep z and w
    vec2i p[2];
    double p_z[2];
    for (int i = 0; i < 2; i++)
    {
        p[i] = this->ndc_to_pixel({p_ndc[i].x, p_ndc[i].y});
        p_z[i] = p_ndc[i].z;
    }

    bool steep = false;
    if (abs(p[1].x-p[0].x) < abs(p[1].y-p[0].y) || p[0].x > p[1].x)
    {
        if (abs(p[1].x-p[0].x) < abs(p[1].y-p[0].y))  // if slope > 1, transpose
        {
            std::swap(p[0].x, p[0].y);
            std::swap(p[1].x, p[1].y);
            steep = true;
        }

        if (p[0].x > p[1].x)  // need to render from left to right
        {
            std::swap(p[0].x, p[1].x);
            std::swap(p[0].y, p[1].y);
        }

        std::swap(p_z[0], p_z[1]);
        std::swap(c[0], c[1]);
    }

    int step = (p[0].y > p[1].y) ? -1 : 1;

    int dx = p[1].x - p[0].x;
    int dy = abs(p[1].y - p[0].y);
    double dz = (p_z[1] - p_z[0]) / dx;

    int x = p[0].x;
    int y = p[0].y;
    double z = p_z[0];

    int D = 2 * dy - dx;

    Color color = c[0];
    Color color_delta = (c[1] - c[0]) / dx;

    if (!steep && x < this->dimensions.x && y < this->dimensions.y && z < this->z_buffer[y][x])
    {
        this->draw_pixel({x, y}, color);
        this->z_buffer[y][x] = z;
    }
    // if steep, de-transpose
    else if (steep && x < this->dimensions.y && y < this->dimensions.x && z < this->z_buffer[x][y])
    {
        this->draw_pixel({y, x}, color);
        this->z_buffer[x][y] = z;
    }

    while (x < p[1].x)
    {
        x++;
        color += color_delta;
        z += dz;

        if (D < 0)
            D += (2 * dy);
        else
        {
            y += step;
            D += (2 * (dy - dx));
        }

        if (!steep && x < this->dimensions.x && y < this->dimensions.y && z < this->z_buffer[y][x])
        {
            this->draw_pixel({x, y}, color);
            this->z_buffer[y][x] = z;
        }
        // if steep, de-transpose
        else if (steep && x < this->dimensions.y && y < this->dimensions.x && z < this->z_buffer[x][y])
        {
            this->draw_pixel({y, x}, color);
            this->z_buffer[x][y] = z;
        }
    }
}

void Window::line(std::vector<vec4d> p_ndc, Color c)
{
    this->line(p_ndc, {c, c});
}

// Vertices' coordinates should be Device Normalized Coordinates (-1 to 1)
// Vertices are assumed to be in counter-clockwise direction
// Uses top-left rasterization rule
// https://www.youtube.com/watch?v=k5wtuKWmV48
void Window::triangle(std::vector<vec4d> v_dnc, std::vector<Color> c,
                      std::vector<vec2d> uv, Texture& texture)
{
    const int decimal_bits = 8;  // because using fixed_24_8, so 8 bits of decimal precision
    // smallest possible value that can be represented with fixed_24_8 (need it to determine bias)
    const fpm::fixed_24_8 smallest_positive = fpm::fixed_24_8::from_raw_value(1);
    const fpm::fixed_24_8 zero_fixed {0};

    // Computes the edge function (cross product of )
    // a, b, p are points
    auto edge_function([](vec4fix24_8 a, vec4fix24_8 b, vec4fix24_8 p)
    {
        // get vectors that represent sides AB and AP
        vec4fix24_8 ab = b - a;
        vec4fix24_8 ap = p - a;
        // get z-component of vec3(ab, 0) x vec3(ap, 0)
        return ((vec2fix24_8) {ab.x, ab.y}).cross_product((vec2fix24_8) {ap.x, ap.y}).z;
    });

    // Helper function to determine whether or not an edge (\vec{start, end})
    // is a flat top or a left one
    // NOTE: using counter-clockwise vertices!
    // `start` and `end` are points
    auto is_top_left([&zero_fixed](vec4fix24_8 start, vec4fix24_8 end)
    {
        vec4fix24_8 edge = end - start;

        bool is_top = (edge.y == zero_fixed && edge.x < zero_fixed);
        bool is_left = (edge.y < zero_fixed);

        return is_top || is_left;
    });

    // Convert device normalized coordinates to pixel coordinates.
    // `z` and `w` remain the same
    vec4fix24_8 v[3];
    for (int i = 0; i < 3; i++)
    {
        vec2i temp = this->ndc_to_pixel({v_dnc[i].x, v_dnc[i].y});
        v[i] = (vec4fix24_8) {fpm::fixed_24_8(temp.x), fpm::fixed_24_8(temp.y), fpm::fixed_24_8(v_dnc[i].z), fpm::fixed_24_8(v_dnc[i].w)};
    }

    // calculate the area of the parallelogram formed by vectors v[0]v[1] and v[0]v[2]
    // needed for barycentic coordinates
    const fpm::fixed_24_8 area = edge_function(v[0], v[1], v[2]);

    // determine the bounding box (min and max values of x and y)
    const vec2i min = {
        (int) fpm::floor(std::min(std::min(v[0].x, v[1].x), v[2].x)),
        (int) fpm::floor(std::min(std::min(v[0].y, v[1].y), v[2].y))
    };
    const vec2i max = {
        (int) fpm::ceil(std::max(std::max(v[0].x, v[1].x), v[2].x)),
        (int) fpm::ceil(std::max(std::max(v[0].y, v[1].y), v[2].y))
    };

    // calculate edge biases (push pixels inside the triangle if they are on
    // the edge that is not a flat top or a left one)
    const fpm::fixed_24_8 bias[3] = {
        (is_top_left(v[0], v[1])) ? zero_fixed : smallest_positive,
        (is_top_left(v[1], v[2])) ? zero_fixed : smallest_positive,
        (is_top_left(v[2], v[0])) ? zero_fixed : smallest_positive,
    };

    // calculate initlial value of edge function
    // for each edge and first point (incremental computation)
    vec2fix24_8 p0 = {fpm::fixed_24_8(min.x + 0.5f), fpm::fixed_24_8(min.y + 0.5f)};
    fpm::fixed_24_8 w_initial[3] = {
        edge_function(v[0], v[1], {p0.x, p0.y, zero_fixed, zero_fixed}) + bias[0],
        edge_function(v[1], v[2], {p0.x, p0.y, zero_fixed, zero_fixed}) + bias[1],
        edge_function(v[2], v[0], {p0.x, p0.y, zero_fixed, zero_fixed}) + bias[2]
    };

    // set delta w constants
    const vec4fix24_8 dw[3] = {
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
        fpm::fixed_24_8 w[3] = {
            w_initial[0],
            w_initial[1],
            w_initial[2]
        };

        for (int x = min.x; x < max.x; x++)
        {
            bool pixel_inside_triangle = (w[0] <= zero_fixed) &&
                                         (w[1] <= zero_fixed) &&
                                         (w[2] <= zero_fixed);

            if (pixel_inside_triangle)
            {
                // calculate barycentric coordinates for this pixel
                // need them both for color interpolation and for Z-interpolation
                // (alpha - v[0], beta - v[1], gamma - v[2])
                double gamma = (double) (w[0] / area);  // <= this is gamma because it shows
                double alpha = (double) (w[1] / area);  // how much I "pull" to the v[2]
                double beta = (double) (w[2] / area);   // in percents, etc. for others

                double pixel_z = ((double) v[0].z * alpha) +
                                 ((double) v[1].z * beta) +
                                 ((double) v[2].z * gamma);

                // prevent out of bounds access of zbuffer (segfault)
                if (y >= 0 && x >= 0 && y < this->dimensions.y && x < this->dimensions.x)
                {
                    // if current pixel is further away that the one already drawn,
                    // proceed to the next one
                    if (pixel_z > this->z_buffer[y][x])
                        continue;
                    // if need to draw current pixel, write its z-value to z-buffer
                    else
                        this->z_buffer[y][x] = pixel_z;
                }
                else  // can't actually draw pixels outside of the screen :(
                    continue;

                // uvw texture coordinates for this pixel
                vec2d pixel_uv = {
                    (uv[0].x * alpha) + (uv[1].x * beta) + (uv[2].x * gamma),
                    (uv[0].y * alpha) + (uv[1].y * beta) + (uv[2].y * gamma)
                };

                // Color color = (c[0] * alpha) + (c[1] * beta) + (c[2] * gamma);
                Color color = texture.get_color_from_uv(pixel_uv);
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

// Device Normalized Coordinates to pixels
vec2i Window::ndc_to_pixel(vec2d v_dnc)
{
    return {
        (int) ((v_dnc.x + 1) * this->dimensions.x) / 2,
        (int) ((1 - v_dnc.y) * this->dimensions.y) / 2,
    };
}

// Reset z_buffer to all +1s (more positive - further away)
void Window::clear_z_buffer()
{
    for (int i = 0; i < this->dimensions.y; i++)
    {
        for (int j = 0; j < this->dimensions.y; j++)
            this->z_buffer[i][j] = 1;
    }
}
