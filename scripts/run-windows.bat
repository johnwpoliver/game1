@echo off
REM Build and run Windows game

set PROJECT_ROOT=%~dp0..
cd /d "%PROJECT_ROOT%\MyGame-Windows"

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
cd /d "%PROJECT_ROOT%"
MyGame-Windows\build\Release\MyGame.exe
