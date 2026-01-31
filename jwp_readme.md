# MyGame Build Instructions

## Project Structure

```
game1/
├── src/
│   └── main.cpp              <- Shared game code
├── MyGame-Android/           <- Android build
└── MyGame-Windows/           <- Windows build
```

---

## Android Build

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
