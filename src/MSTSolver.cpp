#include "MSTSolver.h"

#include "DisjointSet.h"
#include <algorithm>
#include <cmath>

namespace {
    float getDistance(Vector2Int v1, Vector2Int v2);
}

std::vector<Edge> MSTSolver::getEdgesForPoints(std::vector<Vector2Int> points){
    std::vector<Edge> result;
    for (std::size_t i = 0; i < points.size(); i++){
        for (std::size_t j = i + 1; j < points.size(); j++){
            result.push_back(Edge(i, j, getDistance(points[i], points[j])));
        }
    }
    return result;
}

std::vector<Edge> MSTSolver::findMST(int numOfVertices, std::vector<Edge> edges, int extraEdges){
    //Implements Kruskal's algorithm
    std::sort(edges.begin(), edges.end());

    DisjointSet set(numOfVertices);
    std::vector<Edge> result;
    std::vector<Edge> skippedEdges;
    const std::size_t treeEdgeCount = numOfVertices > 0
        ? static_cast<std::size_t>(numOfVertices - 1)
        : 0;

    for (const auto& candidate : edges) {
        if (result.size() < treeEdgeCount && set.unite(candidate.v1, candidate.v2)){
            result.push_back(candidate);
        } else {
            skippedEdges.push_back(candidate);
        }
    }

    const std::size_t requestedExtraEdges = extraEdges > 0
        ? static_cast<std::size_t>(extraEdges)
        : 0;
    const std::size_t extraEdgeCount = std::min(requestedExtraEdges,
                                                skippedEdges.size());
    for (std::size_t i = 0; i < extraEdgeCount; i++) {
        result.push_back(skippedEdges[i]);
    }
    
    return result;
}

namespace {
    float getDistance(Vector2Int v1, Vector2Int v2){
        const float deltaX = static_cast<float>(v1.x - v2.x);
        const float deltaY = static_cast<float>(v1.y - v2.y);
        return std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }
}