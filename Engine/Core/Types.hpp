#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace sokoke {

struct Uuid {
    uint64_t high;
    uint64_t low;
};

typedef uint64_t TicksMS;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;
using IVec2 = glm::ivec2;
using UVec2 = glm::uvec2;

}