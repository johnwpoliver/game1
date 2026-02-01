# MyGame

A learning project built collaboratively with Claude AI, gradually assembling a simple cross-platform game. The focus is on building solid engine elements first (scene management, input handling, display scaling, etc.) with gameplay to follow later.

The same shared source code compiles to both **Windows** and **Android** using **SDL3**.

---

## Project Structure

```
game1/
├── src/                      <- Shared game code
├── tests/                    <- Unit tests (Google Test)
├── scripts/                  <- Build scripts
├── MyGame-Android/           <- Android build
└── MyGame-Windows/           <- Windows build
```

---

## Quick Start (Scripts)

Build scripts are available in both `.bat` (Windows CMD) and `.sh` (Git Bash) formats.

### Run Unit Tests

```powershell
scripts\test.bat
```
```bash
./scripts/test.sh
```

### Build and Run Windows

```powershell
scripts\run-windows.bat
```
```bash
./scripts/run-windows.sh
```

### Build, Install and Run Android

```powershell
scripts\run-android.bat
```
```bash
./scripts/run-android.sh
```

---

## Android Build

### Prerequisites - SDL Symlinks

The Android build requires SDL3 and SDL3_image source code as symlinks in `MyGame-Android/app/jni/`:

```
MyGame-Android/app/jni/
├── SDL -> /c/SDL2/           (SDL3 source, folder name is misleading)
├── SDL_image -> /c/SDL3_image/
└── src/
```

To create the symlinks (from Git Bash or MINGW64):

```bash
cd MyGame-Android/app/jni
ln -s /c/SDL2/ SDL
ln -s /c/SDL3_image/ SDL_image
```

> **Note:** The SDL symlink points to `/c/SDL2/` but this folder actually contains SDL3 source code (v3.5.0).

### Build


```powershell
cd C:\Users\johnw\Documents\github_projects\game1\MyGame-Android
.\gradlew assembleDebug -PBUILD_WITH_CMAKE
```

### Install

```powershell
adb install -r app\build\outputs\apk\debug\app-debug.apk
```

### Start

```powershell
adb shell am start -n org.libsdl.app/.SDLActivity
```

### All-in-one

```powershell
.\gradlew assembleDebug -PBUILD_WITH_CMAKE && adb install -r app\build\outputs\apk\debug\app-debug.apk && adb shell am start -n org.libsdl.app/.SDLActivity
```

### View logs

```powershell
adb logcat -s SDL:V *:E
```

### APK location

```
app\build\outputs\apk\debug\app-debug.apk
```

---

## Windows Build

### First time setup (configure)

```powershell
cd C:\Users\johnw\Documents\github_projects\game1\MyGame-Windows
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Build

```powershell
cmake --build build --config Release
```

### Run

```powershell
.\build\Release\MyGame.exe
```

### EXE location

```
MyGame-Windows\build\Release\MyGame.exe
```

---

## Unit Tests

Uses Google Test for unit testing the shared `src/` code.

### Prerequisites

```powershell
C:\vcpkg\vcpkg install gtest:x64-windows
```

### First time setup (configure)

```powershell
cd C:\Users\johnw\Documents\github_projects\game1\tests
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Build

```powershell
cmake --build build --config Release
```

### Run

```powershell
.\build\Release\MyGameTests.exe
```
