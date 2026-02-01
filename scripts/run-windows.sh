#!/bin/bash
# Build and run Windows game

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/.."
cd "$PROJECT_ROOT/MyGame-Windows"

echo "=== Configuring Windows build ==="
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

echo "=== Building Windows Release ==="
cmake --build build --config Release

echo "=== Running game ==="
cd "$PROJECT_ROOT"
MyGame-Windows/build/Release/MyGame.exe
