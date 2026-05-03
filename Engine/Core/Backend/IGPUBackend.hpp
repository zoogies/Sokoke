#pragma once

namespace sokoke {

class IGPUBackend {
public:
    virtual bool CreateAndBind(void) = 0;
    virtual void TryShutdown(void) = 0;
    virtual void RenderImgui(void) = 0;
    
    // TODO: I think because backend is passed in after being created in game
    //       we might want to not use a destructor and instead use a shutdown
    //       function, because destructor would be called after engine is destroyed
    virtual ~IGPUBackend() = default;
};

}