# MyGame Build Instructions

## Project Structure

```
game1/
├── src/                      <- Shared game code
├── tests/                    <- Unit tests (Google Test)
├── MyGame-Android/           <- Android build
└── MyGame-Windows/           <- Windows build
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
