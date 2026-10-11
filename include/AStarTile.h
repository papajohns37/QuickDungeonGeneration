#pragma once

#include "Vector2Int.h"

struct AStarTile {
    Vector2Int position;
    AStarTile* prev;
    bool valid;
    float distanceFromStart;
    float distanceToEnd;
    float totalHeuristicCost;

    bool operator<(const AStarTile& other) const {
        return totalHeuristicCost < other.totalHeuristicCost;
    }

    bool operator>(const AStarTile& other) const {
        return totalHeuristicCost > other.totalHeuristicCost;
    }
};