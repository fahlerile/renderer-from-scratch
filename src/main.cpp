#include <iostream>
#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Model/Model.hpp"
#include "utils/color.hpp"

void line(vec2i pos0, vec2i pos1, Color color, SDL_Renderer *renderer);

int main()
{
    vec2i dimensions = {800, 800};

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Failed to initialize SDL2 library: " << SDL_GetError() << std::endl;
	    return 1;
    }
    SDL_CreateWindowAndRenderer(dimensions.x, dimensions.y, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    Color white = {255, 255, 255};
    Model head = Model("./res/models/african_head.obj");

    for (int i = 0; i < head.n_faces(); i++)
    {
        vec3i face = head.face(i);
        for (int j = 0; j < 3; j++)
        {
            // -1 because .obj is 1-indexed
            vec3d v0 = head.vertex(face[j] - 1);
            vec3d v1 = head.vertex(face[(j+1)%3] - 1);

            int x0 = (v0.x + 1.) * dimensions.x / 2.;
            int y0 = (v0.y + 1.) * dimensions.y / 2.;
            int x1 = (v1.x + 1.) * dimensions.x / 2.;
            int y1 = (v1.y + 1.) * dimensions.y / 2.;

            line({x0, y0}, {x1, y1}, white, renderer);
        }
    }

    SDL_RenderPresent(renderer);

    while (true)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// Bresenham's algorithm
// See https://www.ics.uci.edu/~gopi/CS112/web/handouts/OldFiles/Bresenham.pdf
void line(vec2i pos0, vec2i pos1, Color color, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, 255);

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
        SDL_RenderDrawPoint(renderer, x, y);
    else  // if steep, de-transpose
        SDL_RenderDrawPoint(renderer, y, x);

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
            SDL_RenderDrawPoint(renderer, x, y);
        else  // if steep, de-transpose
            SDL_RenderDrawPoint(renderer, y, x);
    }
}