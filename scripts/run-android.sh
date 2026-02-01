#!/bin/bash
# Build, install and run Android app

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/../MyGame-Android"

echo "=== Building Android APK ==="
./gradlew assembleDebug -PBUILD_WITH_CMAKE

echo "=== Installing APK ==="
adb install -r app/build/outputs/apk/debug/app-debug.apk

echo "=== Starting app ==="
adb shell am start -n org.libsdl.app/.SDLActivity
