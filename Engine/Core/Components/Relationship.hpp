#pragma once

#include <vector>
#include <Core/Entities/Entity.hpp>

namespace sokoke {

struct Relationship {
    Entity parent; // default-constructed = null (no parent)
    std::vector<Entity> children;
};

}
