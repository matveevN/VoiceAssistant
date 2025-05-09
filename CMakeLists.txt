cmake_minimum_required(VERSION 3.10)
project(voiceAssistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции
option(OPTIMIZE_O3 "Enable -O3 optimization" ON)
option(ENABLE_LTO "Enable Link Time Optimization" ON)
option(ENABLE_PGO "Enable Profile-Guided Optimization" OFF)
option(ENABLE_SANITIZERS "Enable Address/UndefinedBehavior Sanitizers" OFF)

# Пути
set(VOSK_ROOT /usr/local/vosk)
set(PORTAUDIO_ROOT /usr/include)

# Источники
set(SOURCE_FILES
    SourceFiles/main.cpp
    SourceFiles/core/voice_assistant.cpp
    SourceFiles/core/voice_assistant.h
    SourceFiles/speech/speech_recognizer.cpp
    SourceFiles/speech/speech_recognizer.h
    SourceFiles/commands/command_manager.h
    SourceFiles/commands/command_manager.cpp
    SourceFiles/stdafx.h
)

add_executable(voiceAssistant ${SOURCE_FILES})

target_include_directories(voiceAssistant PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/core
    ${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/speech
    ${VOSK_ROOT}
    ${PORTAUDIO_ROOT}
)

target_precompile_headers(voiceAssistant PRIVATE SourceFiles/stdafx.h)

target_link_libraries(voiceAssistant
    ${VOSK_ROOT}/libvosk.so
    portaudio
    pthread
    ${CMAKE_THREAD_LIBS_INIT}
)

# Оптимизации
if(OPTIMIZE_O3)
    target_compile_options(voiceAssistant PRIVATE -O3 -march=native -DNDEBUG)
endif()

if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT LTO_AVAILABLE OUTPUT LTO_ERROR)
    if(LTO_AVAILABLE)
        set_property(TARGET voiceAssistant PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "LTO not supported: ${LTO_ERROR}")
    endif()
endif()

if(ENABLE_PGO)
    target_compile_options(voiceAssistant PRIVATE -fprofile-generate)
    target_link_options(voiceAssistant PRIVATE -fprofile-generate)
endif()

if(ENABLE_SANITIZERS)
    message(STATUS "Sanitizers enabled: address, undefined")
    target_compile_options(voiceAssistant PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer -g)
    target_link_options(voiceAssistant PRIVATE -fsanitize=address,undefined)
endif()
