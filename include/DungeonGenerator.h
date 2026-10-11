#pragma once

#include "DungeonGeneratorConfig.h"
#include "Grid.h"

struct DungeonGenerationResult {
    Grid<uint8_t> map;
    int astarCalls;
};

class DungeonGenerator {
public:
    DungeonGenerationResult generate(
        const DungeonGeneratorConfig& config,
        int seed) const;
};
