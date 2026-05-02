#pragma once

namespace sokoke {

class SokokeEngine
{
public:
    SokokeEngine() = default;
    ~SokokeEngine() = default;

    void Initialize();

    void Tick();
};

}