#include <iostream>

#include <Sokoke/Core/Sokoke.hpp>

int main()
{
    sokoke::SokokeEngine engine;

    engine.Initialize();
    engine.Tick();
    std::cout << "Hello, Sokoke!" << std::endl;
    return 0;
}