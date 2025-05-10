#pragma once
#include "command_manager.h"
#include "speech_recognizer.h"
#include <memory>

namespace Core {
class VoiceAssistant {
public:
        VoiceAssistant();
        void run();

private:
        std::unique_ptr<Speech::SpeechRecognizer> _recognizer;
        Commands::CommandManager _commandManager;

        void processCommand(const std::string& text);
};
} // namespace Core
