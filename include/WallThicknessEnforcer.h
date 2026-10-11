#pragma once

#include "Grid.h"
#include "RandomWalkRoomGeneratorConfig.h"

class WallThicknessEnforcer {
    public:
        void enforceWallThickness(Grid<uint8_t>& map, const RandomWalkRoomGeneratorConfig& config);
};