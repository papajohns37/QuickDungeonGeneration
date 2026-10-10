#pragma once

struct DungeonGeneratorConfig {
    int mapWidth;
    int mapHeight;
    int mapWallValue;
    int mapFloorValue;
    int roomWidth;
    int roomHeight;
    int roomTopPadding;
    int roomBottomPadding;
    int roomLeftPadding;
    int roomRightPadding;
    int roomBrushSize;
    int roomSteps;
    int wallHeightMinimum;
    int wallWidthMinimum;
    int roomTileBase;
    int roomSeedIncrement;
    int corridorWidth;
    int mstExtraEdges;
    float poissonMinimumDistance;
    int poissonPoints;
    int poissonMaximumAttempts;
};
