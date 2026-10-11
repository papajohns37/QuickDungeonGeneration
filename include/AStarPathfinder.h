#pragma once

#include "Grid.h"
#include "Vector2Int.h"
#include <unordered_set>
#include <vector>

class AStarPathfinder{
    public:
        /// @brief Finds a path between two points that only covers valid path tiles.
        /// @param map Map that will be referenced
        /// @param start Start point
        /// @param end End point
        /// @param width Width of the path
        /// @param validPathTiles Valid tiles
        /// @return A list representing the tiles in the path
        std::vector<Vector2Int> FindPath(const Grid<uint8_t>& map, Vector2Int start, Vector2Int end, int checkWidth, int drawWidth, std::unordered_set<int> validPathTiles);
};