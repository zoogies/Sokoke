#include <SDL3/SDL.h>

#include <Backends/SDL/Events.hpp>

namespace sokoke::backends::sdl {

std::vector<PlatformEvent> SDLEventPump::PollEvents() {
    std::vector<PlatformEvent> events;

    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_EVENT_QUIT:
                events.push_back(AppQuitRequestedEvent{});
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                events.push_back(WindowCloseRequestedEvent{ sdlEvent.window.windowID });
                break;
        }
    }

    return events;
}

}
