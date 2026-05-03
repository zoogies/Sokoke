#include <iostream>
#include <type_traits>

#include <Core/Log.hpp>
#include <Core/Sokoke.hpp>

using namespace std;

namespace sokoke {

void SokokeEngine::Initialize(IPlatformBackend *backend) {
    SK_INFO("Initializing Sokoke Engine...");

    running = true;

    this->backend = backend;
    backend->CreateWindow({1280, 720, WindowMode::Windowed, "Sokoke Engine"});

    eventRouter.Register([this](const PlatformEvent& event) {
        return HandlePlatformEvent(event);
    });
}

void SokokeEngine::Tick() {
    if(!running) return;

    for (auto event : backend->Events().PollEvents()) {
        eventRouter.Push(std::move(event));
    }

    eventRouter.Dispatch();
    commandQueue.Flush(*this);
}

void SokokeEngine::Shutdown() {
    SK_INFO("Shutting down Sokoke Engine...");
    running = false;
}

void SokokeEngine::Execute(const QuitCommand&) {
    running = false;
}

bool SokokeEngine::HandlePlatformEvent(const PlatformEvent& event) {
    return std::visit([this](const auto& e) {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, AppQuitRequestedEvent>) {
            commandQueue.Push(QuitCommand{});
            return true;
        }
        else if constexpr (std::is_same_v<T, WindowCloseRequestedEvent>) {
            commandQueue.Push(QuitCommand{});
            return true;
        }
        else {
            return false;
        }
    }, event);
}

}
