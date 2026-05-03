#pragma once

#include <Core/Types.hpp>

#include <Core/Window.hpp>
#include <Core/PlatformEvents.hpp>

namespace sokoke {

class IPlatformBackend {
public:
    virtual IWindow& CreateWindow(const WindowDesc& desc) = 0;
    virtual IPlatformEventPump& Events() = 0;
    virtual TicksMS GetTicks() = 0;
    
    // TODO: I think because backend is passed in after being created in game
    //       we might want to not use a destructor and instead use a shutdown
    //       function, because destructor would be called after engine is destroyed
    virtual ~IPlatformBackend() = default;
};

}