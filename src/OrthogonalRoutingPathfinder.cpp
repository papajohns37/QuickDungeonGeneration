#include "OrthogonalRoutingPathfinder.h"

#include <stdexcept>

namespace{
    bool isBlockedCell(const TileMap& map, int x, int y, int width, const std::unordered_set<int>& validPathTiles){
        for (int dx = -(width / 2); dx < (width + 1) / 2; ++dx) {
            for (int dy = -(width / 2); dy < (width + 1) / 2; ++dy) {
                const int cellX = x + dx;
                const int cellY = y + dy;
                if (cellX < 0 || cellY < 0 || cellX >= map.width || cellY >= map.height) {
                    continue;
                }
                if (validPathTiles.find(map(cellX, cellY)) != validPathTiles.end()) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isValidPath(const TileMap& map, std::vector<Vector2Int> points, int width, std::unordered_set<int> validPathTiles);
    std::vector<Vector2Int> getPath(std::vector<Vector2Int> points, int width);
}

std::vector<Vector2Int> OrthogonalRoutingPathfinder::FindPath(const TileMap& map, Vector2Int start, Vector2Int end, int width, std::unordered_set<int> validPathTiles){
    if (start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0 ||
        start.x >= map.width || start.y >= map.height || end.x >= map.width || end.y >= map.height){
        throw std::invalid_argument("Coordinates must be on the map.");
    }
    
    Vector2Int v1{start.x, end.y};
    Vector2Int v2{end.x, start.y};
    std::vector<Vector2Int> p1 = {start, v1, end};
    std::vector<Vector2Int> p2 = {start, v2, end};
    bool p1Valid = isValidPath(map, p1, width, validPathTiles);
    bool p2Valid = isValidPath(map, p2, width, validPathTiles);
    if (p1Valid && p2Valid){
        //Tiebreaker for now
        if (start.y >= map.height / 2){
            return getPath(p1, width);
        }
        return getPath(p2, width);
    }
    if (p1Valid){
        return getPath(p1, width);
    }
    if (p2Valid){
        return getPath(p2, width);
    }
    return std::vector<Vector2Int>{};
}

namespace{
    bool isValidPath(const TileMap& map, std::vector<Vector2Int> points, int width, std::unordered_set<int> validPathTiles){
        for (size_t i = 0; i < points.size() - 1; i++)
        {
            Vector2Int v1x = points[i].x < points[i + 1].x ? points[i] : points[i + 1];
            Vector2Int v2x = points[i].x >= points[i + 1].x ? points[i] : points[i + 1];
            for (int x = v1x.x; x < v2x.x; x++)
            {
                if (isBlockedCell(map, x, v1x.y, width, validPathTiles)) {
                    return false;
                }
            }
            Vector2Int v1y = points[i].y < points[i + 1].y ? points[i] : points[i + 1];
            Vector2Int v2y = points[i].y >= points[i + 1].y ? points[i] : points[i + 1];
            for (int y = v1y.y; y < v2y.y; y++)
            {
                if (isBlockedCell(map, v1y.x, y, width, validPathTiles)) {
                    return false;
                }
            }
            if (i + 1 < points.size() - 1) {
                const Vector2Int corner = points[i + 1];
                for (int dx = -(width / 2); dx < (width + 1) / 2; ++dx) {
                    for (int dy = -(width / 2); dy < (width + 1) / 2; ++dy) {
                        const int cellX = corner.x + dx;
                        const int cellY = corner.y + dy;
                        if (cellX >= 0 && cellY >= 0 && cellX < map.width && cellY < map.height &&
                            validPathTiles.find(map(cellX, cellY)) != validPathTiles.end()) {
                            return false;
                        }
                    }
                }
            }
        }
        
        return true;
    }

    std::vector<Vector2Int> getPath(std::vector<Vector2Int> points, int width){
        std::vector<Vector2Int> result{};

        for (size_t i = 0; i < points.size() - 1; i++)
        {
            Vector2Int v1x = points[i].x < points[i + 1].x ? points[i] : points[i + 1];
            Vector2Int v2x = points[i].x >= points[i + 1].x ? points[i] : points[i + 1];
            for (int x = v1x.x; x < v2x.x; x++)
            {
                for (int w = -(width / 2); w < (width + 1) / 2; w++)
                {
                    result.push_back(Vector2Int{x + w, v1x.y});
                }
            }
            Vector2Int v1y = points[i].y < points[i + 1].y ? points[i] : points[i + 1];
            Vector2Int v2y = points[i].y >= points[i + 1].y ? points[i] : points[i + 1];
            for (int y = v1y.y; y < v2y.y; y++)
            {
                for (int w = -(width / 2); w < (width + 1) / 2; w++)
                {
                    result.push_back(Vector2Int{v1y.x, y + w});
                }
            }

            if (i + 1 < points.size() - 1) {
                const Vector2Int corner = points[i + 1];
                for (int dx = -(width / 2); dx < (width + 1) / 2; ++dx) {
                    for (int dy = -(width / 2); dy < (width + 1) / 2; ++dy) {
                        result.push_back(Vector2Int{corner.x + dx, corner.y + dy});
                    }
                }
            }
        }

        return result;
    }
}