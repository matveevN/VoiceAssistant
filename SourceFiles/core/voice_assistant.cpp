#include "voice_assistant.h"

namespace Core {
VoiceAssistant::VoiceAssistant() {
        _recognizer = std::make_unique<Speech::SpeechRecognizer>(
            "../../Resources/models/"
            "vosk-model-small-ru-0.22",
            "[\"алиса\", \"включить станок\", \"выключить станок\", \"подать "
            "масло\"]");
}

void VoiceAssistant::processCommand(const std::string& text) {
        _commandManager.process(text);
}

void VoiceAssistant::run() {
        const char* logo = R"(
__     __    _               _            _     _              _   
\ \   / /__ (_) ___ ___     / \   ___ ___(_)___| |_ __ _ _ __ | |_ 
 \ \ / / _ \| |/ __/ _ \   / _ \ / __/ __| / __| __/ _` | '_ \| __|
  \ V / (_) | | (_|  __/  / ___ \\__ \__ \ \__ \ || (_| | | | | |_ 
   \_/ \___/|_|\___\___| /_/   \_\___/___/_|___/\__\__,_|_| |_|\__|

)";

        std::cout << logo;
        std::cout << "Голосовой помощник запущен\n";
        while (true) {
                auto text = _recognizer->listen();
                if (!text.empty()) {
                        processCommand(text);
                }
        }
}

} // namespace Core
