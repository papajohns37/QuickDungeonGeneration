cmake_minimum_required(VERSION 3.20)

project(QuickDungeonGeneration LANGUAGES CXX)

add_executable(QuickDungeonGeneration
    tests/UserInputSeedTest.cpp
    src/DungeonGenerator.cpp
    src/RandomWalkRoomGenerator.cpp
    src/PoissonDiskSampler.cpp
    src/MSTSolver.cpp
    src/DisjointSet.cpp
    src/OrthogonalRoutingPathfinder.cpp
    src/WallThicknessEnforcer.cpp
)

add_executable(RandomWalkRoomGeneratorTests
    tests/RandomWalkRoomGeneratorTests.cpp
    src/RandomWalkRoomGenerator.cpp
    src/WallThicknessEnforcer.cpp
)

add_executable(PoissonDiskSamplerTests
    tests/PoissonDiskSamplerTests.cpp
    src/PoissonDiskSampler.cpp
)

add_executable(MSTSolverTests
    tests/MSTSolverTests.cpp
    src/MSTSolver.cpp
    src/DisjointSet.cpp
)

add_executable(OrthogonalRoutingPathfinderTests
    tests/OrthogonalRoutingPathfinderTests.cpp
    src/OrthogonalRoutingPathfinder.cpp
)

target_include_directories(QuickDungeonGeneration
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(RandomWalkRoomGeneratorTests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(PoissonDiskSamplerTests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(MSTSolverTests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(OrthogonalRoutingPathfinderTests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(QuickDungeonGeneration
    PRIVATE
        cxx_std_20
)

target_compile_features(RandomWalkRoomGeneratorTests
    PRIVATE
        cxx_std_20
)

target_compile_features(PoissonDiskSamplerTests
    PRIVATE
        cxx_std_20
)

target_compile_features(MSTSolverTests
    PRIVATE
        cxx_std_20
)

target_compile_features(OrthogonalRoutingPathfinderTests
    PRIVATE
        cxx_std_20
)

enable_testing()
add_test(NAME RandomWalkRoomGeneratorTests COMMAND RandomWalkRoomGeneratorTests)
add_test(NAME PoissonDiskSamplerTests COMMAND PoissonDiskSamplerTests)
add_test(NAME MSTSolverTests COMMAND MSTSolverTests)
add_test(NAME OrthogonalRoutingPathfinderTests COMMAND OrthogonalRoutingPathfinderTests)