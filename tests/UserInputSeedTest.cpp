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

int main(){
    // Set console output and input encoding to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    RandomWalkRoomGenerator gen;
    RandomWalkRoomGeneratorConfig config{
        MAP_WIDTH,
        MAP_HEIGHT,
        MAP_WIDTH * MAP_HEIGHT / 4,
        BRUSH_SIZE,
        WALL_HEIGHT_MINIMUM,
        WALL_WIDTH_MINIMUM,
        TOP_PADDING,
        BOTTOM_PADDING,
        LEFT_PADDING,
        RIGHT_PADDING,
        Vector2Int{MAP_WIDTH / 2, MAP_HEIGHT / 2}
    };
    int seed;
    std::cout << "Enter a seed: ";
    std::cin >> seed;
    TileMap map(MAP_WIDTH, MAP_HEIGHT);
    gen.generate(config, map, seed);
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