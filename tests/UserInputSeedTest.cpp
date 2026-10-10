#include "DungeonGenerator.h"

#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int seed;
    std::cout << "Enter a seed: ";
    std::cin >> seed;

    const DungeonGeneratorConfig config{
        256,    // mapWidth
        256,    // mapHeight
        1,      // mapWallValue
        0,      // mapFloorValue
        32,    // roomWidth
        32,    // roomHeight
        3,      // roomTopPadding
        3,      // roomBottomPadding
        3,      // roomLeftPadding
        3,      // roomRightPadding
        2,      // roomBrushSize
        512,   // roomSteps
        3,      // wallHeightMinimum
        2,      // wallWidthMinimum
        10,     // roomTileBase
        1,      // roomSeedIncrement
        5,      // corridorWidth
        0,      // mstExtraEdges
        34.0f, // poissonMinimumDistance
        50,     // poissonPoints
        30      // poissonMaximumAttempts
    };

    const DungeonGenerationResult result = DungeonGenerator{}.generate(config, seed);
    std::cout << "Corridors not found: " << result.corridorsNotFound << '\n';
    for (int y = 0; y < config.mapHeight; ++y) {
        for (int x = 0; x < config.mapWidth; ++x) {
            std::cout << (result.map(x, y) == config.mapWallValue ? "██" : "  ");
        }
        std::cout << '\n';
    }
}
