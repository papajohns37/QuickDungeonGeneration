#pragma once

#include "Vector2Int.h"

struct RandomWalkRoomGeneratorConfig {
    int mapWidth;
    int mapHeight;
    int numberOfSteps;
    int brushSize;
    int wallHeightMinimum;
    int wallWidthMinimum;
    int topPadding;
    int bottomPadding;
    int leftPadding;
    int rightPadding;
    Vector2Int startPosition;
};