#include "OrthogonalRoutingPathfinder.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
    void require(bool condition, const std::string& message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    bool containsPoint(const std::vector<Vector2Int>& points, Vector2Int value) {
        for (const auto& point : points) {
            if (point.x == value.x && point.y == value.y) {
                return true;
            }
        }
        return false;
    }

    void testFindPathReturnsStraightRouteWhenNotBlocked() {
        TileMap map(10, 10, 0);
        OrthogonalRoutingPathfinder pathfinder;

        const auto path = pathfinder.FindPath(map, {1, 1}, {5, 4}, 1, {});

        require(!path.empty(), "A valid route should not be empty.");
        require(path.front().x == 1 && path.front().y == 1,
                "The route should begin at the starting tile.");
        require(path.back().x == 5 && path.back().y == 4,
                "The path should end at the requested destination.");
        require(containsPoint(path, {5, 1}) || containsPoint(path, {1, 3}),
                "The route should follow the selected orthogonal branch to the destination.");
    }

    void testFindPathIncludesCornerTilesForThickerRoutes() {
        TileMap map(10, 10, 0);
        OrthogonalRoutingPathfinder pathfinder;

        const auto path = pathfinder.FindPath(map, {1, 8}, {5, 3}, 3, {});

        require(containsPoint(path, {1, 3}), "The route should include the turn corner tile chosen by the route branch.");
        require(containsPoint(path, {5, 3}), "The route should include the destination corner tile.");
        require(containsPoint(path, {1, 7}) || containsPoint(path, {2, 8}),
                "A thick route should include offset tiles along the corridor width.");
        require(containsPoint(path, {2, 3}) || containsPoint(path, {1, 4}),
                "The route should include the filled corner cells at the elbow, not just a single-line segment.");
    }

    void testFindPathUsesWidthPerpendicularToEachSegment() {
        TileMap map(12, 12, 0);
        OrthogonalRoutingPathfinder pathfinder;

        const auto path = pathfinder.FindPath(map, {2, 2}, {8, 8}, 3, {});

        require(containsPoint(path, {4, 1}) && containsPoint(path, {4, 2}) &&
                    containsPoint(path, {4, 3}),
                "Horizontal corridor width should extend perpendicular to travel.");
        require(containsPoint(path, {7, 4}) && containsPoint(path, {8, 4}) &&
                    containsPoint(path, {9, 4}),
                "Vertical corridor width should extend perpendicular to travel.");
    }

    void testFindPathRejectsOutOfBoundsCoordinates() {
        TileMap map(5, 5, 0);
        OrthogonalRoutingPathfinder pathfinder;

        try {
            pathfinder.FindPath(map, {-1, 0}, {2, 2}, 1, {});
            throw std::runtime_error("Out-of-bounds coordinates should throw.");
        } catch (const std::invalid_argument&) {
        }

        try {
            pathfinder.FindPath(map, {0, 0}, {5, 2}, 1, {});
            throw std::runtime_error("Out-of-bounds coordinates should throw.");
        } catch (const std::invalid_argument&) {
        }
    }

    void testFindPathAvoidsBlockedTilesByChoosingAnAlternativeRoute() {
        TileMap map(10, 10, 0);
        map(2, 1) = 1;
        map(3, 1) = 1;
        map(4, 1) = 1;

        OrthogonalRoutingPathfinder pathfinder;
        std::unordered_set<int> blockedTiles = {1};

        const auto path = pathfinder.FindPath(map, {1, 1}, {5, 3}, 1, blockedTiles);

        require(!path.empty(), "The alternative path should be selected when a direct route is blocked.");
        require(path.front().x == 1 && path.front().y == 1,
                "The alternative path should still begin at the start tile.");
        require(!containsPoint(path, {2, 1}) && !containsPoint(path, {3, 1}) && !containsPoint(path, {4, 1}),
                "The path should not travel through any blocked tiles.");
        require(path.back().x == 5 && path.back().y == 3,
                "The path should end at the destination after following the valid orthogonal branch.");
    }
}

int main() {
    try {
        testFindPathReturnsStraightRouteWhenNotBlocked();
        testFindPathIncludesCornerTilesForThickerRoutes();
        testFindPathUsesWidthPerpendicularToEachSegment();
        testFindPathRejectsOutOfBoundsCoordinates();
        testFindPathAvoidsBlockedTilesByChoosingAnAlternativeRoute();
    } catch (const std::exception& error) {
        std::cerr << "OrthogonalRoutingPathfinderTests failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
