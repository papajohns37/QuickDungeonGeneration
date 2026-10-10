#include "MSTSolver.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
    void require(bool condition, const char* message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    bool containsEdge(const std::vector<Edge>& edges, int first, int second) {
        for (const auto& edge : edges) {
            if ((edge.v1 == first && edge.v2 == second) ||
                (edge.v1 == second && edge.v2 == first)) {
                return true;
            }
        }
        return false;
    }

    void testEdgesForPointsBuildCompleteGraphWithEuclideanWeights() {
        MSTSolver solver;
        const auto edges = solver.getEdgesForPoints({
            {0, 0},
            {3, 4},
            {3, 0}
        });

        require(edges.size() == 3,
                "Three points should produce three complete-graph edges.");
        require(containsEdge(edges, 0, 1) && containsEdge(edges, 0, 2) &&
                    containsEdge(edges, 1, 2),
                "The complete graph is missing an expected edge.");

        for (const auto& edge : edges) {
            if (edge.v1 == 0 && edge.v2 == 1) {
                require(std::abs(edge.weight - 5.0f) < 0.001f,
                        "The edge weight should be the Euclidean distance.");
            }
        }
    }

    void testFindMSTSelectsMinimumSpanningTree() {
        MSTSolver solver;
        const std::vector<Edge> edges = {
            {0, 1, 1.0f},
            {1, 2, 2.0f},
            {0, 2, 10.0f},
            {2, 3, 3.0f},
            {1, 3, 8.0f}
        };

        const auto result = solver.findMST(4, edges, 0);

        require(result.size() == 3,
                "A spanning tree of four vertices should contain three edges.");
        require(containsEdge(result, 0, 1) && containsEdge(result, 1, 2) &&
                    containsEdge(result, 2, 3),
                "The solver did not select the minimum spanning tree.");
    }

    void testFindMSTAddsRequestedExtraEdgesAfterTree() {
        MSTSolver solver;
        const std::vector<Edge> edges = {
            {0, 1, 1.0f},
            {1, 2, 2.0f},
            {0, 2, 3.0f}
        };

        const auto result = solver.findMST(3, edges, 1);

        require(result.size() == 3,
                "One extra edge should be added after the spanning tree.");
        require(containsEdge(result, 0, 2),
                "The requested extra cycle edge was not added.");
    }
}

int main() {
    try {
        testEdgesForPointsBuildCompleteGraphWithEuclideanWeights();
        testFindMSTSelectsMinimumSpanningTree();
        testFindMSTAddsRequestedExtraEdgesAfterTree();
    } catch (const std::exception& error) {
        std::cerr << "MSTSolverTests failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
