#pragma once

#include <vector>
#include <functional>

namespace sokoke {

template<typename EventT>
class Signal {
public:
    using Handler = std::function<void(const EventT&)>;

    void Connect(Handler handler) {
        handlers.push_back(std::move(handler));
    }

    void Emit(const EventT& event) {
        for (auto& handler : handlers) {
            handler(event);
        }
    }

private:
    std::vector<Handler> handlers;
};

}