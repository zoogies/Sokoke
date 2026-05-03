#pragma once

#include <cstdint>
#include <entt/entt.hpp>

namespace sokoke {

using EntityID = std::uint32_t;

class Entity {
public:
    Entity() = default;

    explicit Entity(entt::entity handle)
        : handle(handle) {}

    bool IsValid() const {
        return handle != entt::null;
    }

    EntityID ID() const {
        return static_cast<EntityID>(handle);
    }

    entt::entity Raw() const {
        return handle;
    }

    explicit operator bool() const {
        return IsValid();
    }

    bool operator==(const Entity& other) const {
        return handle == other.handle;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

private:
    entt::entity handle{ entt::null };
};

}