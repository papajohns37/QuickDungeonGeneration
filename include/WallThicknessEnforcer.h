#pragma once

#include "RandomWalkRoomGeneratorConfig.h"
#include "TileMap.h"

class WallThicknessEnforcer {
    public:
        void enforceWallThickness(TileMap& map, const RandomWalkRoomGeneratorConfig& config);
};