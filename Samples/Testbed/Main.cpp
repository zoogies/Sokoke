#include <iostream>

#include <Core/Log.hpp>
#include <Core/Sokoke.hpp>
#include <Backends/SDL/SDL.hpp>
#include <Core/Entities/Entity.hpp>
#include <Core/Components/Transform.hpp>

namespace sk = sokoke;

int main()
{
    // sokoke::Log::Init();

    // SK_INFO("Starting Sokoke testbed...");

    // sk::backends::sdl::SDLBackend backend;
    // sk::SokokeEngine engine;

    // engine.Initialize(&backend);
    
    // SK_DEBUG("Hello from Sokoke testbed!");
    
    // auto cube = engine.activeScene.CreateEntity();
    // engine.activeScene.Add<sk::Transform>(cube);

    // auto& transform = engine.activeScene.Get<sk::Transform>(cube);
    // SK_DEBUG("Initial cube position: ({}, {}, {})", transform.position.x, transform.position.y, transform.position.z);

    // // example of using a view
    // for (auto [entity, transform] : engine.activeScene.View<sk::Transform>().each()) {
    //     SK_DEBUG("Entity {} has position ({}, {}, {})", static_cast<uint32_t>(entity), transform.position.x, transform.position.y, transform.position.z);
    // }

    // // engine.activeScene = sk::Scene();
    // // for (auto [entity, transform] : engine.activeScene.View<sk::Transform>().each()) {
    // //     SK_DEBUG("Entity {} has position ({}, {}, {})", static_cast<uint32_t>(entity), transform.position.x, transform.position.y, transform.position.z);
    // // }

    // while(engine.IsRunning())
    // {
    //     engine.Tick();
    // }
    // engine.Shutdown();
    // SK_INFO("Goodbye, Sokoke!");
    return 0;
}