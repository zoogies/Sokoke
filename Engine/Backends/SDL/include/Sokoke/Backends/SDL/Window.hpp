#include <Sokoke/Core/Window.hpp>

struct SDL_Window; // fwd

namespace sokoke::backends::sdl {

class SDLWindow final : public IWindow {
public:
    SDLWindow() = default;
    ~SDLWindow() override;
    void Create(const WindowDesc& desc) override;
private:
    SDL_Window* window = nullptr;
};

}
