cmake_minimum_required(VERSION 3.14)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 23)

# Add executable
add_executable(MazeSolver main.cpp)

# Download rang.hpp using FetchContent
include(FetchContent)
FetchContent_Declare(
    rang
    GIT_REPOSITORY https://github.com/agauniyal/rang.git
    GIT_TAG master
)
FetchContent_MakeAvailable(rang)

# Add include directories
target_include_directories(MazeSolver PRIVATE ${rang_SOURCE_DIR}/include)

