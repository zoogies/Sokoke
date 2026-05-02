#include <SDL3/SDL.h>

#include <Backends/SDL/Window.hpp>

namespace sokoke::backends::sdl {

SDLWindow::~SDLWindow()
{
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void SDLWindow::Create(const WindowDesc& desc)
{
    SDL_WindowFlags flags = 0;
    switch (desc.mode) {
        case WindowMode::Windowed:
            break;
        case WindowMode::Fullscreen:
            flags |= SDL_WINDOW_FULLSCREEN;
            break;
        case WindowMode::Borderless:
            flags |= SDL_WINDOW_BORDERLESS;
            break;
    }
    if (desc.resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    window = SDL_CreateWindow(desc.title.c_str(), desc.width, desc.height, flags);
    id = SDL_GetWindowID(window);
}

}
