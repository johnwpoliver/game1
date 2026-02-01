@echo off
REM Build, install and run Android app

cd /d "%~dp0..\MyGame-Android"

echo === Building Android APK ===
call gradlew.bat assembleDebug -PBUILD_WITH_CMAKE
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo === Installing APK ===
adb install -r app\build\outputs\apk\debug\app-debug.apk
if errorlevel 1 (
    echo Install failed!
    exit /b 1
)

echo === Starting app ===
adb shell am start -n org.libsdl.app/.SDLActivity
