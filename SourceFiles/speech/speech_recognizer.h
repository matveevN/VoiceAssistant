#pragma once

#include <portaudio.h>
#include <vosk_api.h>

namespace Speach {
class SpeechRecognizer {
public:
        SpeechRecognizer(const std::string& modelPath,
                         const std::string& grammarJson);
        ~SpeechRecognizer();

        std::string listen();

private:
        VoskModel* _model = nullptr;
        VoskRecognizer* _recognizer = nullptr;
        PaStream* _stream = nullptr;
        PaStreamParameters _inputParameters{};
};
} // namespace Speach
