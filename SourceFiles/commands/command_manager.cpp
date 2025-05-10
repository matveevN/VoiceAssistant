#include "command_manager.h"
#include <chrono>
#include <iomanip>

namespace Commands {
CommandManager::CommandManager() {
        registerCommand("start_machine", "включить станок", []() {
                std::cout << "Станок включен\n";
        });
        registerCommand("stop_machine", "выключить станок", []() {
                std::cout << "Станок выключен\n";
        });
        registerCommand("oil_feed", "подать масло", []() {
                std::cout << "Масло подано\n";
        });
}

void CommandManager::process(const std::string& text) {
        if (text.find("алиса") == std::string::npos) {
                return;
        }

        for (const auto& [name, data] : _commands) {
                if (text.find(data.voiceTrigger) != std::string::npos) {
                        logTime();
                        data.handler();
                        return;
                }
        }
}

void CommandManager::logTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "["
                  << std::put_time(std::localtime(&now_time_t),
                                   "%Y-%m-%d %H:%M:%S")
                  << "] ";
}
} // namespace Commands
