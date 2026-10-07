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
    src/RandomWalkRoomGenerator.cpp
)

add_executable(RandomWalkRoomGeneratorTests
    tests/RandomWalkRoomGeneratorTests.cpp
    src/RandomWalkRoomGenerator.cpp
)

target_include_directories(QuickDungeonGeneration
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(RandomWalkRoomGeneratorTests
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

target_compile_features(RandomWalkRoomGeneratorTests
    PRIVATE
        cxx_std_20
)

enable_testing()
add_test(NAME RandomWalkRoomGeneratorTests COMMAND RandomWalkRoomGeneratorTests)