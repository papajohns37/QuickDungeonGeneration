#pragma once

#include "Vector2Int.h"

struct RandomWalkRoomGeneratorConfig {
    int mapWidth;
    int mapHeight;
    int numberOfSteps;
    int brushSize;
    int wallHeightMinimum;
    int xPadding;
    int yPadding;
    Vector2Int startPosition;
};