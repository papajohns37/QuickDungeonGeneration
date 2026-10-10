#pragma once

#include "DungeonGeneratorConfig.h"
#include "TileMap.h"

struct DungeonGenerationResult {
    TileMap map;
    int corridorsNotFound;
};

class DungeonGenerator {
public:
    DungeonGenerationResult generate(
        const DungeonGeneratorConfig& config,
        int seed) const;
};
