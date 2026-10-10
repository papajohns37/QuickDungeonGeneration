#pragma once

#include "Edge.h"
#include "Vector2Int.h"
#include <vector>

class DelaunayTriangulation{
    std::vector<Edge> getEdgesForPoints(std::vector<Vector2Int> points);
};