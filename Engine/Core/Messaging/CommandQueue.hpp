#pragma once

#include <utility>
#include <variant>
#include <vector>

namespace sokoke {

struct QuitCommand {};

using Command = std::variant<
    QuitCommand
>;

class CommandQueue {
public:
    void Push(Command command) {
        commands.push_back(std::move(command));
    }

    template<typename Executor>
    void Flush(Executor& executor) {
        for (auto& command : commands) {
            std::visit([&](auto& c) {
                executor.Execute(c);
            }, command);
        }

        commands.clear();
    }

private:
    std::vector<Command> commands;
};

}
