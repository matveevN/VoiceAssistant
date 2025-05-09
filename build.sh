#!/bin/bash

set -e

PROJECT_NAME="voiceAssistant"
BUILD_DIR="build"

# Этап 1: сборка для PGO
build_release_pgo_gen() {
    echo "=== [1/3] Release (PGO - генерация профиля)... ==="
    cmake -B ${BUILD_DIR}/release_pgo -DCMAKE_BUILD_TYPE=Release \
          -DOPTIMIZE_O3=ON -DENABLE_LTO=ON -DENABLE_PGO=ON
    cmake --build ${BUILD_DIR}/release_pgo -j$(nproc)

    echo "[+] Выполни бинарник для генерации профиля:"
    echo "    ./${BUILD_DIR}/release_pgo/${PROJECT_NAME}"
    echo "    Потом запусти этот скрипт снова с флагом '--pgo-use'"
}

# Этап 2: сборка с использованием профиля
build_release_pgo_use() {
    echo "=== [2/3] Release (PGO - использование профиля)... ==="
    cmake -B ${BUILD_DIR}/release -DCMAKE_BUILD_TYPE=Release \
          -DOPTIMIZE_O3=ON -DENABLE_LTO=ON -DENABLE_PGO=OFF
    cmake --build ${BUILD_DIR}/release -j$(nproc)
    echo "[✓] Release билд с оптимизациями готов: ./${BUILD_DIR}/release/${PROJECT_NAME}"
}

# Debug билд с санитайзерами
build_debug_sanitized() {
    echo "=== [3/3] Debug (санитайзеры)... ==="
    cmake -B ${BUILD_DIR}/debug -DCMAKE_BUILD_TYPE=RelWithDebInfo \
          -DOPTIMIZE_O3=OFF -DENABLE_LTO=OFF -DENABLE_SANITIZERS=ON
    cmake --build ${BUILD_DIR}/debug -j$(nproc)
    echo "[✓] Debug билд с санитайзерами готов: ./${BUILD_DIR}/debug/${PROJECT_NAME}"
}

case "$1" in
    release)
        build_release_pgo_gen
        ;;
    --pgo-use)
        build_release_pgo_use
        ;;
    debug)
        build_debug_sanitized
        ;;
    all)
        build_release_pgo_gen
        build_debug_sanitized
        ;;
    *)
        echo "Использование: $0 [release | --pgo-use | debug | all]"
        ;;
esac

