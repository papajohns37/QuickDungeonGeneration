#include "MSTSolver.h"
#include "OrthogonalRoutingPathfinder.h"
#include "PoissonDiskSampler.h"
#include "TileMap.h"
#include "RandomWalkRoomGenerator.h"
#include "RandomWalkRoomGeneratorConfig.h"
#include "Vector2Int.h"
#include <iostream>
#include <unordered_set>
#include <windows.h>

constexpr int MAP_WIDTH = 64;
constexpr int MAP_HEIGHT = 64;
constexpr int BRUSH_SIZE = 2;
constexpr int WALL_HEIGHT_MINIMUM = 3;
constexpr int WALL_WIDTH_MINIMUM = 3;
constexpr int TOP_PADDING = 3;
constexpr int BOTTOM_PADDING = 3;
constexpr int LEFT_PADDING = 3;
constexpr int RIGHT_PADDING = 3;

constexpr float POISSON_MIN_DISTANCE = 25.0f;
constexpr int POISSON_POINTS = 15;
constexpr int POISSON_MAX_ATTEMPTS = 30;

int main(){
    // Set console output and input encoding to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // RandomWalkRoomGenerator gen;
    // RandomWalkRoomGeneratorConfig config{
    //     MAP_WIDTH,
    //     MAP_HEIGHT,
    //     MAP_WIDTH * MAP_HEIGHT / 4,
    //     BRUSH_SIZE,
    //     WALL_HEIGHT_MINIMUM,
    //     WALL_WIDTH_MINIMUM,
    //     TOP_PADDING,
    //     BOTTOM_PADDING,
    //     LEFT_PADDING,
    //     RIGHT_PADDING,
    //     Vector2Int{MAP_WIDTH / 2, MAP_HEIGHT / 2}
    // };
    int seed;
    std::cout << "Enter a seed: ";
    std::cin >> seed;
    TileMap map(MAP_WIDTH, MAP_HEIGHT);
    // gen.generate(config, map, seed);
    PoissonDiskSampler sampler;
    const auto samples = sampler.generateIntegerPoints(MAP_WIDTH, MAP_HEIGHT, POISSON_MIN_DISTANCE, POISSON_POINTS, POISSON_MAX_ATTEMPTS, seed);

    MSTSolver solver;
    const auto mstEdges = solver.findMST(
        static_cast<int>(samples.size()),
        solver.getEdgesForPoints(samples),
        3);

    std::unordered_set<int> validPathTiles;
    int pathsNotFound = 0;

    for (const auto& sample : samples) {
        map(sample.x, sample.y) = 1; // Mark the sampled point on the map
    }

    for (const auto& edge : mstEdges) {
        const Vector2Int start = samples[edge.v1];
        const Vector2Int end = samples[edge.v2];
        const auto path = OrthogonalRoutingPathfinder{}.FindPath(map, start, end, 1, validPathTiles);

        if (path.empty()) {
            ++pathsNotFound;
            continue;
        }

        for (const auto& point : path) {
            if (point.x >= 0 && point.x < MAP_WIDTH && point.y >= 0 && point.y < MAP_HEIGHT) {
                if (map(point.x, point.y) == 0) {
                    map(point.x, point.y) = 2;
                }
            }
        }
    }

    std::cout << "Paths not found: " << pathsNotFound << std::endl;

    for (int i = 0; i < MAP_HEIGHT; i++){
        for (int j = 0; j < MAP_WIDTH; j++){
            if (map(j, i) == 1){
                std::cout << "██";
            } else if (map(j, i) == 2){
                std::cout << "░░";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}