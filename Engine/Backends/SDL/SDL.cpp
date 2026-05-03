#include <SDL3/SDL.h>

#include <Core/Log.hpp>
#include <Backends/SDL/SDL.hpp>

namespace sokoke::backends::sdl {

SDLBackend::SDLBackend()
{
    SDL_Init(SDL_INIT_VIDEO);
    SK_INFO("Initialized SDL backend.");
}

SDLBackend::~SDLBackend()
{
    SDL_Quit();
    SK_INFO("Shut down SDL backend.");
}

SDLWindow& SDLBackend::CreateWindow(const WindowDesc& desc)
{
    auto window = std::make_unique<SDLWindow>();
    window->Create(desc);
    SDLWindow& ref = *window;
    windows.push_back(std::move(window));

    SK_INFO("Created window '{}' ({}x{}, mode: {})", desc.title, desc.width, desc.height, static_cast<int>(desc.mode));

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

void SDLBackend::AddEventSink(ISDLEventSink* sink)
{
    eventPump.AddEventSink(sink);
}

}
