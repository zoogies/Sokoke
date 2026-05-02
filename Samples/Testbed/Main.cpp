#include <iostream>

#include <Core/Sokoke.hpp>
#include <Backends/SDL/SDL.hpp>

namespace sk = sokoke;

int main()
{
    sk::backends::sdl::SDLBackend backend;
    sk::SokokeEngine engine;

    std::cout << "Hello, Sokoke!" << std::endl;

    engine.Initialize(&backend);
    while(engine.IsRunning())
    {
        engine.Tick();
    }
    engine.Shutdown();
    std::cout << "Goodbye, Sokoke!" << std::endl;
    return 0;
}