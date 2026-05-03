#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"

#include <Core/Log.hpp>
#include <Core/Sokoke.hpp>
#include <Backends/SDL/SDL.hpp>
#include <Backends/SDL_GPU/SDL_GPU.hpp>
#include <Core/Entities/Entity.hpp>
#include <Core/Components/Transform.hpp>

#include <Panels/Panels.hpp>

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

    while(engine.IsRunning())
    {
        engine.Tick();

        ImGuiID dsId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        static bool s_layoutDone = false;
        if (!s_layoutDone) {
            s_layoutDone = true;
            ImGui::DockBuilderRemoveNode(dsId);
            ImGui::DockBuilderAddNode(dsId, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dsId, ImGui::GetMainViewport()->WorkSize);
            ImGuiID dockLeft, dockCenter;
            ImGui::DockBuilderSplitNode(dsId, ImGuiDir_Left, 1.0f / 6.0f, &dockLeft, &dockCenter);
            ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
            ImGui::DockBuilderFinish(dsId);
        }

        editor::PaintHierarchyPanel(engine.activeScene);
    }
    engine.Shutdown();
    SK_INFO("Goodbye, Sokoke!");
    return 0;
}