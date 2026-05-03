#include <iostream>

#include "imgui.h"

#include <Core/Log.hpp>
#include <Core/Sokoke.hpp>
#include <Backends/SDL/SDL.hpp>
#include <Backends/SDL_GPU/SDL_GPU.hpp>
#include <Core/Entities/Entity.hpp>
#include <Core/Components/Transform.hpp>

namespace sk = sokoke;

int main()
{
    sokoke::Log::Init();

    SK_INFO("Starting Sokoke editor...");

    sk::backends::sdl::SDLBackend backend;
    sk::backends::sdl_gpu::SDL_GPUBackend gpu_backend;
    backend.AddEventSink(&gpu_backend);

    sk::SokokeEngine engine;

    engine.Initialize(&backend, &gpu_backend);
    
    SK_DEBUG("Hello from Sokoke editor!");
    
    auto cube = engine.activeScene.CreateEntity();
    engine.activeScene.Add<sk::Transform>(cube);

    auto& transform = engine.activeScene.Get<sk::Transform>(cube);
    SK_DEBUG("Initial cube position: ({}, {}, {})", transform.position.x, transform.position.y, transform.position.z);

    // example of using a view
    for (auto [entity, transform] : engine.activeScene.View<sk::Transform>().each()) {
        SK_DEBUG("Entity {} has position ({}, {}, {})", static_cast<uint32_t>(entity), transform.position.x, transform.position.y, transform.position.z);
    }

    // engine.activeScene = sk::Scene();
    // for (auto [entity, transform] : engine.activeScene.View<sk::Transform>().each()) {
    //     SK_DEBUG("Entity {} has position ({}, {}, {})", static_cast<uint32_t>(entity), transform.position.x, transform.position.y, transform.position.z);
    // }

    bool show_demo_window = true;
    bool show_another_window = false;

    while(engine.IsRunning())
    {
        engine.Tick();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::End();
        }

        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }
    engine.Shutdown();
    SK_INFO("Goodbye, Sokoke!");
    return 0;
}