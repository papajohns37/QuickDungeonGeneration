#pragma once

#include "TileMap.h"
#include "Vector2Int.h"
#include <unordered_set>
#include <vector>

class OrthogonalRoutingPathfinder {
    public:
        /// @brief Finds a path between with a width and 1 turn at most, that only crosses valid tiles
        /// @param map TileMap to traverse
        /// @param start Start point
        /// @param end End point
        /// @param width Width of the path
        /// @param validPathTiles Valid tiles the path occupy
        /// @return All tiles in the path, empty set if no such path exists
        std::vector<Vector2Int> FindPath(const TileMap& map, Vector2Int start, Vector2Int end, int width, std::unordered_set<int> validPathTiles);
};