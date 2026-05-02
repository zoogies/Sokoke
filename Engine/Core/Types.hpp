#pragma once

#include <cstdint>

namespace sokoke {

struct Uuid {
    uint64_t high;
    uint64_t low;
};

typedef uint64_t TicksMS;

}