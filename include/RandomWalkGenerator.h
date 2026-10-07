#pragma once

#include <raylib.h>

class RandomWalkGenerator {
    public:
        void generate(int mapWidth, int mapHeight, int numberOfSteps, int brushSize, Vector2 startPosition, int** map, int seed);
};