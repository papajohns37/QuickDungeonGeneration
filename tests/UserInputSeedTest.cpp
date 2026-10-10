#include "PoissonDiskSampler.h"
#include "TileMap.h"
#include "RandomWalkRoomGenerator.h"
#include "RandomWalkRoomGeneratorConfig.h"
#include "Vector2Int.h"
#include <iostream>
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

constexpr float POISSON_MIN_DISTANCE = 30.0f;
constexpr int POISSON_POINTS = 10;
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

    for (const auto& sample : samples) {
        map(sample.x, sample.y) = 1; // Mark the sampled point on the map
    }

    for (int i = 0; i < MAP_HEIGHT; i++){
        for (int j = 0; j < MAP_WIDTH; j++){
            if (map(j, i) == 1){
                std::cout << "██";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}