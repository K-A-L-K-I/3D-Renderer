cmake_minimum_required(VERSION 3.16)
project(CGFromScratch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.30.12
    GIT_SHALLOW    TRUE
)

set(SDL_SHARED   OFF CACHE BOOL "" FORCE)
set(SDL_STATIC   ON  CACHE BOOL "" FORCE)
set(SDL_TEST     OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2)

add_executable(renderer src/main.cpp)
target_link_libraries(renderer PRIVATE SDL2::SDL2-static SDL2::SDL2main)
