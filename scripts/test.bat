@echo off
REM Build and run all unit tests

cd /d "%~dp0..\tests"

echo === Configuring tests ===
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
if errorlevel 1 (
    echo Configuration failed!
    exit /b 1
)

echo === Building tests ===
cmake --build build --config Release
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo === Running tests ===
.\build\Release\MyGameTests.exe
