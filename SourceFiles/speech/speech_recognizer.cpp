#include "speech_recognizer.h"
#include <iostream>

namespace Speech {
constexpr int sampleRate = 16000;
constexpr int framesPerBuffer = 4000;

SpeechRecognizer::SpeechRecognizer(const std::string& modelPath,
                                   const std::string& grammarJson) {
        _model = vosk_model_new(modelPath.c_str());
        _recognizer = vosk_recognizer_new_grm(_model,
                                              sampleRate,
                                              grammarJson.c_str());

        PaError err = Pa_Initialize();
        if (err != paNoError) {
                std::cerr << "Ошибка инициализации PortAudio: "
                          << Pa_GetErrorText(err) << "\n";
                exit(1);
        }

        _inputParameters.device = Pa_GetDefaultInputDevice();
        if (_inputParameters.device == paNoDevice) {
                std::cerr << "Нет доступного входного устройства.\n";
                exit(1);
        }

        _inputParameters.channelCount = 1;
        _inputParameters.sampleFormat = paInt16;
        _inputParameters.suggestedLatency
            = Pa_GetDeviceInfo(_inputParameters.device)->defaultLowInputLatency;
        _inputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(&_stream,
                            &_inputParameters,
                            nullptr,
                            sampleRate,
                            framesPerBuffer,
                            paClipOff,
                            nullptr,
                            nullptr);
        if (err != paNoError) {
                std::cerr << "Ошибка открытия потока: " << Pa_GetErrorText(err)
                          << "\n";
                exit(1);
        }

        Pa_StartStream(_stream);
}

std::string SpeechRecognizer::listen() {
        short buffer[framesPerBuffer];
        Pa_ReadStream(_stream, buffer, framesPerBuffer);

        if (vosk_recognizer_accept_waveform(_recognizer,
                                            reinterpret_cast<const char*>(
                                                buffer),
                                            framesPerBuffer * sizeof(short))) {
                const char* result = vosk_recognizer_result(_recognizer);
                std::string text(result);

                auto pos1 = text.find("\"text\" : \"");
                if (pos1 != std::string::npos) {
                        pos1 += 10;
                        auto pos2 = text.find("\"", pos1);
                        return text.substr(pos1, pos2 - pos1);
                }
        }
        return "";
}

SpeechRecognizer::~SpeechRecognizer() {
        Pa_StopStream(_stream);
        Pa_CloseStream(_stream);
        Pa_Terminate();

        vosk_recognizer_free(_recognizer);
        vosk_model_free(_model);
}
} // namespace Speech
