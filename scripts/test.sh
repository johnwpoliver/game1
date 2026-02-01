#!/bin/bash
# Build and run all unit tests

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/../tests"

echo "=== Configuring tests ==="
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

echo "=== Building tests ==="
cmake --build build --config Release

echo "=== Running tests ==="
./build/Release/MyGameTests.exe
