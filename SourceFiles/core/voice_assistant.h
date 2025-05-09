// voiceassistant.h
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
        std::unique_ptr<Speach::SpeechRecognizer> recognizer;
        CommandManager commandManager; // Всё управление командами здесь

        void processCommand(const std::string& text);
};
} // namespace Core
