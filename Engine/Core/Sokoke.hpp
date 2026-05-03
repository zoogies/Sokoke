#pragma once

#include <Core/IBackend.hpp>
#include <Core/Entities/Scene.hpp>
#include <Core/Messaging/CommandQueue.hpp>
#include <Core/Messaging/EventPump.hpp>

namespace sokoke {

class SokokeEngine
{
public:
    SokokeEngine() = default;
    ~SokokeEngine() = default;

    void Initialize(IPlatformBackend *backend);

    void Tick();

    void Shutdown();

    bool IsRunning() const { return running; }

    void Execute(const QuitCommand& command);

    Scene activeScene;
private:
    bool HandlePlatformEvent(const PlatformEvent& event);

    bool running = false;
    IPlatformBackend* backend = nullptr;
    EventRouter eventRouter;
    CommandQueue commandQueue;
};

}
