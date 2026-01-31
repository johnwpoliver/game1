cmake_minimum_required(VERSION 3.20)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

add_executable(MyGame src/main.cpp)
target_link_libraries(MyGame 
    SDL2::SDL2 
    SDL2::SDL2main
    SDL2_image::SDL2_image
)
