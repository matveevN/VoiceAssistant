#pragma once

#include <functional>
#include <unordered_map>

namespace Commands {
class CommandManager {
public:
        CommandManager();

        void process(const std::string& text);

private:
        struct CommandData {
                std::string voiceTrigger;
                std::function<void()> handler;
        };

        std::unordered_map<std::string, CommandData> _commands;

        template<typename F>
        void registerCommand(std::string name,
                             std::string voiceTrigger,
                             F&& handler) {
                _commands.emplace(std::move(name),
                                  CommandData{std::move(voiceTrigger),
                                              std::forward<F>(handler)});
        }

        void logTime();
};
} // namespace Commands
