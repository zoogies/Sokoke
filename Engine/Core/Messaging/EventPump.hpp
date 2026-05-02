#pragma once

#include <functional>
#include <vector>

#include <Core/PlatformEvents.hpp>

namespace sokoke {

class EventRouter {
public:
    using Handler = std::function<bool(const PlatformEvent&)>;

    void Register(Handler handler) {
        handlers.push_back(std::move(handler));
    }

    void Push(PlatformEvent event) {
        queue.push_back(std::move(event));
    }

    void Dispatch() {
        for (const auto& event : queue) {
            for (auto& handler : handlers) {
                if (handler(event)) {
                    break;
                }
            }
        }

        queue.clear();
    }

private:
    std::vector<Handler> handlers;
    std::vector<PlatformEvent> queue;
};

}
