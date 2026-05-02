#pragma once

#include <Core/Window.hpp>
#include <Core/PlatformEvents.hpp> // WindowID

struct SDL_Window; // fwd

namespace sokoke::backends::sdl {

class SDLWindow final : public IWindow {
public:
    SDLWindow() = default;
    ~SDLWindow() override;
    void Create(const WindowDesc& desc) override;
private:
    SDL_Window* window = nullptr;
    WindowID id = 0;
};

}
