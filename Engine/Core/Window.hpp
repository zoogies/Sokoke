#pragma once

#include <string>
#include <Core/Types.hpp>

namespace sokoke {

enum WindowMode {
    Windowed,
    Fullscreen,
    Borderless
};

struct WindowDesc {
    int width = 1280;
    int height = 720;
    WindowMode mode = WindowMode::Windowed;
    std::string title = "Sokoke";
    // Uuid icon; TODO
    bool resizable = true;
};

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void Create(const WindowDesc& desc) = 0;
};

}