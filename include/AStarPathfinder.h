#pragma once

#include "TileMap.h"
#include "Vector2Int.h"
#include <unordered_set>
#include <vector>

class AStarPathfinder{
    public:
        std::vector<Vector2Int> FindPath(const TileMap& map, Vector2Int start, Vector2Int end, int width, std::unordered_set<int> validPathTiles);
};