#include "imgui.h"

#include <Panels/Panels.hpp>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

using namespace sokoke;

namespace editor {

static entt::entity s_selected        = entt::null;
static entt::entity s_renaming        = entt::null;
static entt::entity s_clipboard       = entt::null;
static entt::entity s_dragHoverEntity = entt::null;
static double       s_dragHoverStart  = 0.0;
static char         s_searchBuf[128]  = {};
static char         s_renameBuf[256]  = {};

// ─── Helpers ──────────────────────────────────────────────────────────────────

static std::string EntityLabel(Scene& scene, entt::entity e) {
    if (auto* n = scene.TryGet<Name>(Entity{ e }))
        if (!n->value.empty())
            return n->value;
    return "Entity " + std::to_string(static_cast<uint32_t>(e));
}

static bool IsDescendantOf(Scene& scene, entt::entity node, entt::entity potentialAncestor) {
    Entity e{ node };
    while (auto* rel = scene.TryGet<Relationship>(e)) {
        if (!rel->parent.IsValid()) break;
        if (rel->parent.Raw() == potentialAncestor) return true;
        e = rel->parent;
    }
    return false;
}

static void Detach(Scene& scene, Entity child) {
    auto* rel = scene.TryGet<Relationship>(child);
    if (!rel || !rel->parent.IsValid()) return;
    if (auto* pRel = scene.TryGet<Relationship>(rel->parent)) {
        auto& ch = pRel->children;
        ch.erase(std::remove(ch.begin(), ch.end(), child), ch.end());
    }
    rel->parent = Entity{};
}

static void Attach(Scene& scene, Entity child, Entity parent) {
    Detach(scene, child);
    if (!scene.Has<Relationship>(child)) scene.Add<Relationship>(child);
    if (!scene.Has<Relationship>(parent)) scene.Add<Relationship>(parent);
    scene.Get<Relationship>(child).parent = parent;
    scene.Get<Relationship>(parent).children.push_back(child);
}

static void DeleteTree(Scene& scene, Entity entity) {
    std::vector<Entity> children;
    if (auto* rel = scene.TryGet<Relationship>(entity))
        children = rel->children;
    for (auto& child : children)
        if (scene.IsValid(child))
            DeleteTree(scene, child);
    Detach(scene, entity);
    if (entity.Raw() == s_selected)  s_selected  = entt::null;
    if (entity.Raw() == s_renaming)  s_renaming  = entt::null;
    if (entity.Raw() == s_clipboard) s_clipboard = entt::null;
    scene.DestroyEntity(entity);
}

static Entity DuplicateTree(Scene& scene, Entity source, Entity parent) {
    Entity copy = scene.CreateEntity();

    // Snapshot children before Attach — if parent == source, Attach adds copy into
    // source's children list, causing the post-Attach snapshot to include copy itself
    // and recurse infinitely.
    std::vector<Entity> childrenToClone;
    if (auto* rel = scene.TryGet<Relationship>(source))
        childrenToClone = rel->children;

    if (scene.Has<Transform>(source)) {
        auto t = scene.Get<Transform>(source);
        scene.Add<Transform>(copy) = t;
    }
    if (auto* n = scene.TryGet<Name>(source)) {
        std::string name = n->value;
        scene.Add<Name>(copy).value = name + " (Copy)";
    }
    if (parent.IsValid())
        Attach(scene, copy, parent);

    for (auto& child : childrenToClone)
        if (scene.IsValid(child))
            DuplicateTree(scene, child, copy);

    return copy;
}

static void StartRename(Scene& scene, entt::entity e) {
    s_renaming = e;
    std::string label = EntityLabel(scene, e);
    snprintf(s_renameBuf, sizeof(s_renameBuf), "%s", label.c_str());
}

// ─── Node ─────────────────────────────────────────────────────────────────────

static void PaintNode(Scene& scene, entt::entity e, bool flatMode) {
    Entity entity{ e };
    if (!scene.IsValid(entity)) return;

    std::string label = EntityLabel(scene, e);

    bool hasChildren = !flatMode && [&] {
        auto* rel = scene.TryGet<Relationship>(entity);
        return rel && !rel->children.empty();
    }();

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow    |
        ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_FramePadding;
    if (!hasChildren)    flags |= ImGuiTreeNodeFlags_Leaf;
    if (e == s_selected) flags |= ImGuiTreeNodeFlags_Selected;

    ImGui::PushID(static_cast<int>(e));

    // Auto-expand when a drag has been hovering over this node long enough
    if (hasChildren && ImGui::GetDragDropPayload()) {
        if (s_dragHoverEntity == e && (ImGui::GetTime() - s_dragHoverStart) >= 0.6)
            ImGui::SetNextItemOpen(true, ImGuiCond_Always);
    }

    bool open = ImGui::TreeNodeEx("##n", flags, "%s", label.c_str());

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
        s_selected = e;

    if (e == s_selected && ImGui::IsKeyPressed(ImGuiKey_F2))
        StartRename(scene, e);

    // Track which node the drag is hovering over for the auto-expand timer
    if (ImGui::GetDragDropPayload()) {
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
            if (s_dragHoverEntity != e) {
                s_dragHoverEntity = e;
                s_dragHoverStart  = ImGui::GetTime();
            }
        } else if (s_dragHoverEntity == e) {
            s_dragHoverEntity = entt::null;
        }
    } else {
        s_dragHoverEntity = entt::null;
    }

    // Drag source
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("SK_ENTITY", &e, sizeof(entt::entity));
        ImGui::Text("Move: %s", label.c_str());
        ImGui::EndDragDropSource();
    }

    // Drop target: reparent dragged entity under this one
    if (ImGui::BeginDragDropTarget()) {
        if (auto* p = ImGui::AcceptDragDropPayload("SK_ENTITY")) {
            entt::entity dragged = *static_cast<const entt::entity*>(p->Data);
            if (dragged != e && !IsDescendantOf(scene, e, dragged))
                Attach(scene, Entity{ dragged }, entity);
        }
        ImGui::EndDragDropTarget();
    }

    // Right-click context menu
    bool deleted = false;
    if (ImGui::BeginPopupContextItem("##ctx")) {
        if (ImGui::MenuItem("Create Child")) {
            Entity child = scene.CreateEntity();
            scene.Add<Transform>(child);
            Attach(scene, child, entity);
            s_selected = child.Raw();
        }
        if (ImGui::MenuItem("Rename"))
            StartRename(scene, e);

        if (ImGui::MenuItem("Create Empty Parent")) {
            Entity newParent = scene.CreateEntity();
            scene.Add<Transform>(newParent);
            // Slot new entity where this one was in the hierarchy
            auto* rel = scene.TryGet<Relationship>(entity);
            Entity grandparent = (rel && rel->parent.IsValid()) ? rel->parent : Entity{};
            if (grandparent.IsValid())
                Attach(scene, newParent, grandparent);
            Attach(scene, entity, newParent);
            s_selected = newParent.Raw();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Copy"))
            s_clipboard = e;

        if (s_clipboard != entt::null && scene.IsValid(Entity{ s_clipboard })) {
            if (ImGui::MenuItem("Paste as Sibling")) {
                auto* rel = scene.TryGet<Relationship>(entity);
                Entity parent = (rel && rel->parent.IsValid()) ? rel->parent : Entity{};
                s_selected = DuplicateTree(scene, Entity{ s_clipboard }, parent).Raw();
            }
            if (ImGui::MenuItem("Paste as Child"))
                s_selected = DuplicateTree(scene, Entity{ s_clipboard }, entity).Raw();
        }

        if (ImGui::MenuItem("Duplicate")) {
            auto* rel = scene.TryGet<Relationship>(entity);
            Entity parent = (rel && rel->parent.IsValid()) ? rel->parent : Entity{};
            s_selected = DuplicateTree(scene, entity, parent).Raw();
        }

        ImGui::Separator();

        if (auto* rel = scene.TryGet<Relationship>(entity); rel && rel->parent.IsValid())
            if (ImGui::MenuItem("Unparent"))
                Detach(scene, entity);

        ImGui::Separator();

        if (ImGui::MenuItem("Delete")) {
            DeleteTree(scene, entity);
            deleted = true;
        }
        ImGui::EndPopup();
    }

    if (open) {
        if (!deleted && !flatMode) {
            std::vector<Entity> children;
            if (auto* rel = scene.TryGet<Relationship>(entity))
                children = rel->children;
            for (auto& child : children)
                PaintNode(scene, child.Raw(), false);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}

// ─── Panel ────────────────────────────────────────────────────────────────────

void PaintHierarchyPanel(Scene& scene) {
    ImGui::Begin("Hierarchy");

    // Keyboard shortcuts (Ctrl+D duplicate, Ctrl+C copy, Ctrl+V paste)
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) {
        bool ctrl = ImGui::GetIO().KeyCtrl;
        if (ctrl && ImGui::IsKeyPressed(ImGuiKey_C) && s_selected != entt::null)
            s_clipboard = s_selected;
        if (ctrl && ImGui::IsKeyPressed(ImGuiKey_D) && s_selected != entt::null && scene.IsValid(Entity{ s_selected })) {
            Entity src{ s_selected };
            auto* rel = scene.TryGet<Relationship>(src);
            Entity parent = (rel && rel->parent.IsValid()) ? rel->parent : Entity{};
            s_selected = DuplicateTree(scene, src, parent).Raw();
        }
        if (ctrl && ImGui::IsKeyPressed(ImGuiKey_V) && s_clipboard != entt::null && scene.IsValid(Entity{ s_clipboard }))
            s_selected = DuplicateTree(scene, Entity{ s_clipboard }, Entity{}).Raw(); // paste at root
    }

    // Search bar + add button
    float btnW = 26.0f;
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - btnW - ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputTextWithHint("##search", "Search...", s_searchBuf, sizeof(s_searchBuf));
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(btnW, 0))) {
        Entity e = scene.CreateEntity();
        scene.Add<Transform>(e);
        s_selected = e.Raw();
    }

    ImGui::Separator();

    // Rename modal — opened from context menu or F2
    if (s_renaming != entt::null && scene.IsValid(Entity{ s_renaming }))
        ImGui::OpenPopup("Rename##modal");

    if (ImGui::BeginPopupModal("Rename##modal", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
        bool commit = ImGui::InputText("##rename_in", s_renameBuf, sizeof(s_renameBuf), ImGuiInputTextFlags_EnterReturnsTrue);
        if (commit || ImGui::Button("OK")) {
            Entity ent{ s_renaming };
            if (!scene.Has<Name>(ent)) scene.Add<Name>(ent);
            scene.Get<Name>(ent).value = s_renameBuf;
            s_renaming = entt::null;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            s_renaming = entt::null;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Entity list
    ImGui::BeginChild("##list", ImVec2(0, 0), false);

    std::string search{ s_searchBuf };
    std::transform(search.begin(), search.end(), search.begin(), [](unsigned char c) { return std::tolower(c); });

    if (search.empty()) {
        for (auto [e, transform] : scene.View<Transform>().each()) {
            auto* rel = scene.TryGet<Relationship>(Entity{ e });
            if (!rel || !rel->parent.IsValid())
                PaintNode(scene, e, false);
        }
    } else {
        for (auto [e, transform] : scene.View<Transform>().each()) {
            std::string label = EntityLabel(scene, e);
            std::transform(label.begin(), label.end(), label.begin(), [](unsigned char c) { return std::tolower(c); });
            if (label.find(search) != std::string::npos)
                PaintNode(scene, e, true);
        }
    }

    // Drop zone at bottom: drag here to unparent (make root)
    ImVec2 avail = ImGui::GetContentRegionAvail();
    if (avail.y < 4.0f) avail.y = 4.0f;
    ImGui::InvisibleButton("##drop_root", ImVec2(-1.0f, avail.y));
    if (ImGui::BeginDragDropTarget()) {
        if (auto* p = ImGui::AcceptDragDropPayload("SK_ENTITY")) {
            entt::entity dragged = *static_cast<const entt::entity*>(p->Data);
            Detach(scene, Entity{ dragged });
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::EndChild();
    ImGui::End();
}

} // namespace editor
