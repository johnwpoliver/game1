cmake_minimum_required(VERSION 3.6)

project(my_app)

if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 CONFIG REQUIRED)
endif()

if(NOT TARGET SDL3_image::SDL3_image)
    find_package(SDL3_image CONFIG REQUIRED)
endif()

add_library(main SHARED
    ../../../../src/main.cpp
    ../../../../src/IntroScene.cpp
    ../../../../src/LevelIntroScene.cpp
    ../../../../src/PlayingScene.cpp
    ../../../../src/GameOverScene.cpp
    ../../../../src/FPSCounter.cpp
    ../../../../src/PerformanceMonitor.cpp
    ../../../../src/Character1.cpp
)

target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
