#pragma once

#include <ctime>
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

        void registerCommand(const std::string& name,
                             const std::string& voiceTrigger,
                             std::function<void()> handler);

        void logTime();
};
} // namespace Commands
