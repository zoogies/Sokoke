#include <iostream>

#include <Sokoke/Core/Sokoke.hpp>

namespace sk = sokoke;

int main()
{
    sk::SokokeEngine engine;

    engine.Initialize();
    engine.Tick();
    std::cout << "Hello, Sokoke!" << std::endl;
    return 0;
}