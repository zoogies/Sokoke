#include <SDL3/SDL.h>

#include <Backends/SDL/SDL.hpp>

namespace sokoke::backends::sdl {

SDLBackend::SDLBackend()
{
    SDL_Init(SDL_INIT_VIDEO);
}

SDLBackend::~SDLBackend()
{
    SDL_Quit();
}

SDLWindow& SDLBackend::CreateWindow(const WindowDesc& desc)
{
    auto window = std::make_unique<SDLWindow>();
    window->Create(desc);
    SDLWindow& ref = *window;
    windows.push_back(std::move(window));
    return ref;
}

SDLEventPump& SDLBackend::Events()
{
    return eventPump;
}

TicksMS SDLBackend::GetTicks()
{
    return SDL_GetTicks();
}

}
