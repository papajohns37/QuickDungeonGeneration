cmake_minimum_required(VERSION 3.20)

project(QuickDungeonGeneration LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
)

FetchContent_MakeAvailable(raylib)

add_executable(QuickDungeonGeneration
    tests/UserInputSeedTest.cpp
    src/RandomWalkGenerator.cpp
)

target_include_directories(QuickDungeonGeneration
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(QuickDungeonGeneration
    PRIVATE
        raylib
)

target_compile_features(QuickDungeonGeneration
    PRIVATE
        cxx_std_20
)