@echo off
REM Build and run Windows game

cd /d "%~dp0..\MyGame-Windows"

echo === Configuring Windows build ===
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
if errorlevel 1 (
    echo Configuration failed!
    exit /b 1
)

echo === Building Windows Release ===
cmake --build build --config Release
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo === Running game ===
.\build\Release\MyGame.exe
