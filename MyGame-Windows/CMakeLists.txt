cmake_minimum_required(VERSION 3.20)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(MyGame
    ../src/main.cpp
    ../src/IntroScene.cpp
    ../src/LevelIntroScene.cpp
    ../src/PlayingScene.cpp
    ../src/GameOverScene.cpp
    ../src/FPSCounter.cpp
    ../src/PerformanceMonitor.cpp
    ../src/Character1.cpp
    ../src/Input.cpp
    ../src/DisplayManager.cpp
)

target_link_libraries(MyGame PRIVATE
    SDL3::SDL3
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
)
