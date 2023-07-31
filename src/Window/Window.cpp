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