#pragma once

#include <SDL3/SDL.h>

#include <Backends/SDL/ISDLEventSink.hpp>
#include <Core/Backend/IGPUBackend.hpp>

namespace sokoke::backends::sdl_gpu {

class SDL_GPUBackend final : public IGPUBackend, public sdl::ISDLEventSink
{
public:
    SDL_GPUBackend() = default;
    ~SDL_GPUBackend() = default;

    bool CreateAndBind(void) override;
    void TryShutdown(void) override;
    void RenderImgui(void) override;

    bool ProcessSDLEvent(const SDL_Event& event) override;
private:
    SDL_GPUDevice* gpu_device = nullptr;
    SDL_Window* window = nullptr; // TODO: IT IS ABSOLUTELY VILE TO DOUBLE TRACK THIS HERE!!!
};

}
