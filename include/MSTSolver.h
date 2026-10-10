#pragma once

#include "Edge.h"
#include "Vector2Int.h"
#include <vector>

class MSTSolver {
    public:
        std::vector<Edge> getEdgesForPoints(std::vector<Vector2Int> points);
        std::vector<Edge> findMST(int numOfVertices, std::vector<Edge> edges, int extraEdges);
};