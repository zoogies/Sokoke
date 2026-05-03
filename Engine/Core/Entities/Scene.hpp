#pragma once

#include <entt/entt.hpp>
#include <utility>

#include <Core/Entities/Entity.hpp>

namespace sokoke {

template<typename... Components>
struct Exclude {};

class Scene {
public:
    Entity CreateEntity() {
        return Entity{ registry.create() };
    }

    void DestroyEntity(Entity entity) {
        if (IsValid(entity)) {
            registry.destroy(entity.Raw());
        }
    }

    bool IsValid(Entity entity) const {
        return entity.IsValid() && registry.valid(entity.Raw());
    }

    template<typename T, typename... Args>
    T& Add(Entity entity, Args&&... args) {
        return registry.emplace<T>(entity.Raw(), std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    T& AddOrReplace(Entity entity, Args&&... args) {
        return registry.emplace_or_replace<T>(entity.Raw(), std::forward<Args>(args)...);
    }

    template<typename T>
    T& Replace(Entity entity, T&& component) {
        return registry.replace<T>(entity.Raw(), std::forward<T>(component));
    }

    template<typename T, typename... Args>
    T& Patch(Entity entity, Args&&... args) {
        return registry.patch<T>(entity.Raw(), std::forward<Args>(args)...);
    }

    template<typename T>
    T& Get(Entity entity) {
        return registry.get<T>(entity.Raw());
    }

    template<typename T>
    const T& Get(Entity entity) const {
        return registry.get<T>(entity.Raw());
    }

    template<typename T>
    T* TryGet(Entity entity) {
        return registry.try_get<T>(entity.Raw());
    }

    template<typename T>
    const T* TryGet(Entity entity) const {
        return registry.try_get<T>(entity.Raw());
    }

    template<typename T>
    bool Has(Entity entity) const {
        return registry.all_of<T>(entity.Raw());
    }

    template<typename... Components>
    bool HasAll(Entity entity) const {
        return registry.all_of<Components...>(entity.Raw());
    }

    template<typename... Components>
    bool HasAny(Entity entity) const {
        return registry.any_of<Components...>(entity.Raw());
    }

    // Safe remove: removes T if present, does nothing if missing.
    template<typename T>
    void Remove(Entity entity) {
        registry.remove<T>(entity.Raw());
    }

    // Strict erase: expects T to exist.
    template<typename T>
    void Erase(Entity entity) {
        registry.erase<T>(entity.Raw());
    }

    template<typename... Include>
    auto View() {
        return registry.view<Include...>();
    }

    template<typename... Include>
    auto View() const {
        return registry.view<Include...>();
    }

    template<typename... Include, typename... ExcludeComponents>
    auto View(Exclude<ExcludeComponents...>) {
        return registry.view<Include...>(entt::exclude<ExcludeComponents...>);
    }

    template<typename... Include, typename... ExcludeComponents>
    auto View(Exclude<ExcludeComponents...>) const {
        return registry.view<Include...>(entt::exclude<ExcludeComponents...>);
    }

    template<typename... Include>
    auto Each() {
        return registry.view<Include...>().each();
    }

    template<typename... Include>
    auto Each() const {
        return registry.view<Include...>().each();
    }

    template<typename... Include, typename... ExcludeComponents>
    auto Each(Exclude<ExcludeComponents...> exclude) {
        return View<Include...>(exclude).each();
    }

    template<typename... Include, typename... ExcludeComponents>
    auto Each(Exclude<ExcludeComponents...> exclude) const {
        return View<Include...>(exclude).each();
    }

    entt::registry& Registry() {
        return registry;
    }

    const entt::registry& Registry() const {
        return registry;
    }

private:
    entt::registry registry;
};

}