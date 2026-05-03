#include <iostream>

#include <Core/Sokoke.hpp>
#include <Backends/SDL/SDL.hpp>
#include <Core/Entities/Entity.hpp>
#include <Core/Components/Transform.hpp>

namespace sk = sokoke;

int main()
{
    sk::backends::sdl::SDLBackend backend;
    sk::SokokeEngine engine;

    std::cout << "Hello, Sokoke!" << std::endl;

    engine.Initialize(&backend);

    auto cube = engine.activeScene.CreateEntity();
    engine.activeScene.Add<sk::Transform>(cube);

    auto& transform = engine.activeScene.Get<sk::Transform>(cube);
    std::cout << "Initial cube position: (" << transform.position.x << ", " << transform.position.y << ", " << transform.position.z << ")" << std::endl;

    // example of using a view
    for (auto [entity, transform] : engine.activeScene.View<sk::Transform>().each()) {
        std::cout << "Entity " << static_cast<uint32_t>(entity) << " has position (" << transform.position.x << ", " << transform.position.y << ", " << transform.position.z << ")" << std::endl;
    }

    // engine.activeScene = sk::Scene();
    // for (auto [entity, transform] : engine.activeScene.View<sk::Transform>().each()) {
    //     std::cout << "Entity " << static_cast<uint32_t>(entity) << " has position (" << transform.position.x << ", " << transform.position.y << ", " << transform.position.z << ")" << std::endl;
    // }

    while(engine.IsRunning())
    {
        engine.Tick();
    }
    engine.Shutdown();
    std::cout << "Goodbye, Sokoke!" << std::endl;
    return 0;
}