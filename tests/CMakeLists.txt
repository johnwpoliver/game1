cmake_minimum_required(VERSION 3.20)
project(MyGameTests)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

# Test executable
add_executable(MyGameTests
    test_character1.cpp
    test_displaymanager.cpp
    test_scenemanager.cpp
    test_playingscene.cpp
    test_lives.cpp
    test_score.cpp
    ../src/Character1.cpp
    ../src/DisplayManager.cpp
    ../src/Input.cpp
    ../src/PlayingScene.cpp
    ../src/GameOverScene.cpp
    ../src/IntroScene.cpp
    ../src/LevelIntroScene.cpp
    ../src/Lives.cpp
    ../src/Score.cpp
)

target_include_directories(MyGameTests PRIVATE ../src)

target_link_libraries(MyGameTests PRIVATE
    GTest::gtest
    GTest::gtest_main
    SDL3::SDL3
)

include(GoogleTest)
gtest_discover_tests(MyGameTests)
