#include "RandomWalkRoomGenerator.h"
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

namespace {
    void applyBrush(Vector2Int position, RandomWalkRoomGeneratorConfig config, int** map);
    void deleteTile(Vector2Int position, RandomWalkRoomGeneratorConfig config, int** map);
    Vector2Int positionPlusDirection(Vector2Int startPosition, int direction);
    bool brushCanBeApplied(Vector2Int pos, RandomWalkRoomGeneratorConfig config);
    bool isValidPosition(Vector2Int pos, RandomWalkRoomGeneratorConfig config);
}

void RandomWalkRoomGenerator::generate(RandomWalkRoomGeneratorConfig config, int** map, int seed) {
    //Validate input parameters
    if (config.mapWidth <= 0){
        throw std::invalid_argument("Map width must be positive.");
    }
    if (config.mapHeight <= 0){
        throw std::invalid_argument("Map height must be positive.");
    }
    if (config.brushSize <= 0){
        throw std::invalid_argument("Brush size must be positive.");
    }
    if (config.wallHeightMinimum <= 0){
        throw std::invalid_argument("Wall height minimum must be positive.");
    }
    if (config.xPadding < 0){
        throw std::invalid_argument("X Padding cannot be negative.");
    }
    if (config.yPadding < 0){
        throw std::invalid_argument("Y Padding cannot be negative.");
    }
    if (config.numberOfSteps < 0){
        throw std::invalid_argument("Number of steps cannot be negative.");
    }
    if (!map) {
        throw std::invalid_argument("Map is a null pointer.");
    }
    for (int y = 0; y < config.mapHeight; y++) {
        if (!map[y]) {
            throw std::invalid_argument("Row" + std::to_string(y) + " in map was not allocated.");
        }
    }
    if (config.xPadding > std::numeric_limits<int>::max() / 2 || config.yPadding > std::numeric_limits<int>::max() / 2){
        throw std::invalid_argument("X or Y padding are too large.");
    }
    int effectiveWidth = config.mapWidth - config.xPadding * 2;
    int effectiveHeight = config.mapHeight - config.yPadding * 2;
    if (effectiveWidth <= config.brushSize){
        throw std::invalid_argument("Effective width is too small.");
    }
    if (effectiveHeight <= config.brushSize){
        throw std::invalid_argument("Effective height is too small.");
    }
    if (!brushCanBeApplied(config.startPosition, config)){
        throw std::invalid_argument("Starting position is not valid.");
    }
    if (config.yPadding < config.wallHeightMinimum){
        throw std::invalid_argument("Y Padding must be at least the wall height minimum to avoid issues near top of map.");
    }

    // Populate the map with walls
    for (int y = 0; y < config.mapHeight; y++) {
        for (int x = 0; x < config.mapWidth; x++) {
            map[y][x] = 1;
        }
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> directionDist(0, 3);

    Vector2Int position = config.startPosition;
    applyBrush(position, config, map);
    for (int i = 0; i < config.numberOfSteps; i++){
        int direction;
        do {
            direction = directionDist(rng);
        } while (!brushCanBeApplied(positionPlusDirection(position, direction), config));

        position = positionPlusDirection(position, direction);

        applyBrush(position, config, map);
    }
}


namespace {
    void applyBrush(Vector2Int position, RandomWalkRoomGeneratorConfig config, int** map){
        if (!brushCanBeApplied(position, config)){
            return;
        }
        for (int x = position.x - config.brushSize / 2; x < position.x - config.brushSize / 2 + config.brushSize; x++){
            for (int y = position.y - config.brushSize / 2; y < position.y - config.brushSize / 2 + config.brushSize; y++){
                deleteTile(Vector2Int(x, y), config, map);
            }
        }
    }


    void deleteTile(Vector2Int position, RandomWalkRoomGeneratorConfig config, int** map){
        if (!brushCanBeApplied(Vector2Int(position.x, position.y), config)){
            return;
        }
        map[position.y][position.x] = 0;

        int x = position.x;
        int y1 = position.y;

        //Check if above violates wall height minimum
        bool deleteTilesAbove = false;
        for (int y = y1 + 1; y < y1 + config.wallHeightMinimum; y++){
            if (!isValidPosition(Vector2Int(x, y + 1), config)){
                break;
            }
            if (map[y][x] == 1 && map[y + 1][x] == 0){
                deleteTilesAbove = true;
                break;
            }
            if (map[y][x] == 0){
                break;
            }
        }

        //Check if below violates wall height minimum
        bool deleteTilesBelow = false;
        for (int y = y1 - 1; y > y1 - config.wallHeightMinimum; y--){
            if (!isValidPosition(Vector2Int(x, y - 1), config)){
                break;
            }
            if (map[y][x] == 1 && map[y - 1][x] == 0){
                deleteTilesBelow = true;
                break;
            }
            if (map[y][x] == 0){
                break;
            }
        }

        //Delete tiles as necessary
        for (int y = y1 + 1; y < y1 + config.wallHeightMinimum && deleteTilesAbove; y++){
            if (!isValidPosition(Vector2Int(x, y), config)){
                break;
            }
            if (map[y][x] == 0){
                break;
            }
            map[y][x] = 0;
        }
        for (int y = y1 - 1; y > y1 - config.wallHeightMinimum && deleteTilesBelow; y--){
            if (!isValidPosition(Vector2Int(x, y), config)){
                break;
            }
            if (map[y][x] == 0){
                break;
            }
            map[y][x] = 0;
        }
    }

    Vector2Int positionPlusDirection(Vector2Int startPosition, int direction){
        switch (direction){
            case 0: return Vector2Int(startPosition.x, startPosition.y + 1);
            case 1: return Vector2Int(startPosition.x, startPosition.y - 1);
            case 2: return Vector2Int(startPosition.x + 1, startPosition.y);
            case 3: return Vector2Int(startPosition.x - 1, startPosition.y);
        }

        return startPosition; // Should never reach here
    }

    bool brushCanBeApplied(Vector2Int pos, RandomWalkRoomGeneratorConfig config){
        for (int x = static_cast<int>(pos.x) - config.brushSize / 2; x < static_cast<int>(pos.x) - config.brushSize / 2 + config.brushSize; x++){
            for (int y = static_cast<int>(pos.y) - config.brushSize / 2; y < static_cast<int>(pos.y) - config.brushSize / 2 + config.brushSize; y++){
                if (!isValidPosition(Vector2Int(x, y), config)){
                    return false;
                }
            }
        }
        return true;
    }

    bool isValidPosition(Vector2Int pos, RandomWalkRoomGeneratorConfig config){
        return pos.x >= config.xPadding && pos.x < config.mapWidth - config.xPadding && pos.y >= config.yPadding && pos.y < config.mapHeight - config.yPadding;
    }
}