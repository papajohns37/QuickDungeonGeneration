#include "AStarPathfinder.h"

#include "AStarTile.h"
#include <functional>
#include <limits>
#include <queue>

namespace {
    bool tileIsValid(const Grid<uint8_t>& map, int width, std::unordered_set<int> validPathTiles, Vector2Int tile);
    float distance(Vector2Int v1, Vector2Int v2);
}

std::vector<Vector2Int> AStarPathfinder::FindPath(const Grid<uint8_t>& map, Vector2Int start, Vector2Int end, int checkWidth, int drawWidth, std::unordered_set<int> validPathTiles){
    Grid<AStarTile> astarMap(map.width, map.height);
    for (int x = 0; x < map.width; x++)
    {
        for (int y = 0; y < map.height; y++)
        {
            astarMap(x, y) = AStarTile{
                Vector2Int{x, y},
                nullptr,
                tileIsValid(map, checkWidth, validPathTiles, Vector2Int{x, y}),
                std::numeric_limits<float>::infinity(),
                distance(Vector2Int{x, y}, end),
                std::numeric_limits<float>::infinity()
            };
        }
    }
    astarMap(start.x, start.y).distanceFromStart = 0.0f;
    astarMap(start.x, start.y).totalHeuristicCost = distance(start, end);

    std::priority_queue<AStarTile, std::vector<AStarTile>, std::greater<AStarTile>> queue;
    queue.push(astarMap(start.x, start.y));
    while (!queue.empty()){
        AStarTile current = queue.top();
        queue.pop();

        AStarTile& left = astarMap(current.position.x - 1, current.position.y);
        AStarTile& right = astarMap(current.position.x + 1, current.position.y);
        AStarTile& bottom = astarMap(current.position.x, current.position.y - 1);
        AStarTile& top = astarMap(current.position.x, current.position.y + 1);
        std::reference_wrapper<AStarTile> adjacent[] = {left, right, bottom, top};

        for (int i = 0; i < 4; i++)
        {
            if (!adjacent[i].get().valid){
                continue;
            }

            float distanceFromStart = current.distanceFromStart + 1;
            if (distanceFromStart < adjacent[i].get().distanceFromStart){
                adjacent[i].get().distanceFromStart = distanceFromStart;
                adjacent[i].get().totalHeuristicCost = distanceFromStart + adjacent[i].get().distanceToEnd;
                adjacent[i].get().prev = &astarMap(current.position.x, current.position.y);
                queue.push(adjacent[i].get());
            }
        }
    }

    if (!astarMap(end.x, end.y).prev){
        return std::vector<Vector2Int>{};
    }

    std::unordered_set<int> visitedTiles;
    std::vector<Vector2Int> result;
    AStarTile* current = &astarMap(end.x, end.y);

    while (current != nullptr)
    {
        Vector2Int currentPosition = current->position;
        for (int dx = -(drawWidth / 2); dx < (drawWidth + 1) / 2; ++dx) {
            for (int dy = -(drawWidth / 2); dy < (drawWidth + 1) / 2; ++dy) {
                int nx = currentPosition.x + dx;
                int ny = currentPosition.y + dy;

                int tileIndex = ny * map.width + nx;
                    
                // insert().second is true only if the element was newly inserted
                if (visitedTiles.insert(tileIndex).second) {
                    result.push_back(Vector2Int{nx, ny});
                }
            }
        }
        current = current->prev;
    }

    return result;
}

namespace {
    bool tileIsValid(const Grid<uint8_t>& map, int width, std::unordered_set<int> validPathTiles, Vector2Int tile){
        for (int dx = -(width / 2); dx < (width + 1) / 2; ++dx) {
            for (int dy = -(width / 2); dy < (width + 1) / 2; ++dy) {
                Vector2Int current{tile.x + dx, tile.y + dy};
                if (current.x < 0 || current.x >= map.width || current.y < 0 || current.y >= map.height){
                    return false;
                }
                if (validPathTiles.find(map(current.x, current.y)) != validPathTiles.end()){
                    return false;
                }
            }
        }

        return true;
    }
    float distance(Vector2Int v1, Vector2Int v2) {
        float dx = v1.x - v2.x;
        float dy = v2.y - v2.y;
        return std::sqrt(dx * dx + dy * dy);
    }
}