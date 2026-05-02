#include <Sokoke/Core/Sokoke.hpp>

#include <iostream>

namespace sokoke {

void SokokeEngine::Initialize() {
    std::cout << "Initializing Sokoke Engine..." << std::endl;
}

void SokokeEngine::Tick() {
    std::cout << "Ticking Sokoke Engine..." << std::endl;
}

}