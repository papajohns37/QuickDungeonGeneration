#include "RandomWalkRoomGenerator.h"
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

namespace {
    void applyBrush(Vector2Int position, const RandomWalkRoomGeneratorConfig& config, TileMap& map);
    void enforceMinimumWallHeight(const RandomWalkRoomGeneratorConfig& config, TileMap& map);
    void enforceMinimumWallWidth(const RandomWalkRoomGeneratorConfig& config, TileMap& map);
    Vector2Int positionPlusDirection(Vector2Int startPosition, int direction);
    bool brushCanBeApplied(Vector2Int pos, const RandomWalkRoomGeneratorConfig& config);
    bool isValidPosition(Vector2Int pos, const RandomWalkRoomGeneratorConfig& config);
}

void RandomWalkRoomGenerator::generate(const RandomWalkRoomGeneratorConfig& config, TileMap& map, int seed) {
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
    if (config.wallWidthMinimum <= 0){
        throw std::invalid_argument("Wall width minimum must be positive.");
    }
    if (config.topPadding < 0){
        throw std::invalid_argument("Top padding cannot be negative.");
    }
    if (config.bottomPadding < 0){
        throw std::invalid_argument("Bottom padding cannot be negative.");
    }
    if (config.leftPadding < 0){
        throw std::invalid_argument("Left padding cannot be negative.");
    }
    if (config.rightPadding < 0){
        throw std::invalid_argument("Right padding cannot be negative.");
    }
    if (config.numberOfSteps < 0){
        throw std::invalid_argument("Number of steps cannot be negative.");
    }
    if (config.topPadding < config.wallHeightMinimum){
        throw std::invalid_argument("Top padding must be at least the wall height minimum.");
    }
    if (config.bottomPadding < config.wallHeightMinimum){
        throw std::invalid_argument("Bottom padding must be at least the wall height minimum.");
    }
    if (config.leftPadding < config.wallWidthMinimum){
        throw std::invalid_argument("Left padding must be at least the wall width minimum.");
    }
    if (config.rightPadding < config.wallWidthMinimum){
        throw std::invalid_argument("Right padding must be at least the wall width minimum.");
    }
    if (config.mapWidth != map.width || config.mapHeight != map.height){
        throw std::invalid_argument("Map dimensions do not match configuration.");
    }
    int effectiveWidth = config.mapWidth - config.leftPadding - config.rightPadding;
    int effectiveHeight = config.mapHeight - config.topPadding - config.bottomPadding;
    if (effectiveWidth <= config.brushSize){
        throw std::invalid_argument("Effective width is too small.");
    }
    if (effectiveHeight <= config.brushSize){
        throw std::invalid_argument("Effective height is too small.");
    }
    if (!brushCanBeApplied(config.startPosition, config)){
        throw std::invalid_argument("Starting position is not valid.");
    }

    // Populate the map with walls
    for (int y = 0; y < config.mapHeight; y++) {
        for (int x = 0; x < config.mapWidth; x++) {
            map(x, y) = 1;
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

    enforceMinimumWallWidth(config, map);
    enforceMinimumWallHeight(config, map);
}


namespace {
    void applyBrush(Vector2Int position, const RandomWalkRoomGeneratorConfig& config, TileMap& map){
        if (!brushCanBeApplied(position, config)){
            return;
        }
        for (int x = position.x - config.brushSize / 2; x < position.x - config.brushSize / 2 + config.brushSize; x++){
            for (int y = position.y - config.brushSize / 2; y < position.y - config.brushSize / 2 + config.brushSize; y++){
                map(x, y) = 0;
            }
        }
    }

    void deleteTile(Vector2Int position, const RandomWalkRoomGeneratorConfig& config, TileMap& map){
        if (!isValidPosition(position, config)){
            return;
        }
        map(position.x, position.y) = 0;

        int x = position.x;
        int y1 = position.y;

        //Check if above violates wall height minimum
        bool deleteTilesAbove = false;
        for (int y = y1 + 1; y < y1 + config.wallHeightMinimum; y++){
            if (!isValidPosition(Vector2Int(x, y + 1), config)){
                break;
            }
            if (map(x, y) == 1 && map(x, y + 1) == 0){
                deleteTilesAbove = true;
                break;
            }
            if (map(x, y) == 0){
                break;
            }
        }

        //Check if below violates wall height minimum
        bool deleteTilesBelow = false;
        for (int y = y1 - 1; y > y1 - config.wallHeightMinimum; y--){
            if (!isValidPosition(Vector2Int(x, y - 1), config)){
                break;
            }
            if (map(x, y) == 1 && map(x, y - 1) == 0){
                deleteTilesBelow = true;
                break;
            }
            if (map(x, y) == 0){
                break;
            }
        }

        //Delete tiles as necessary
        for (int y = y1 + 1; y < y1 + config.wallHeightMinimum && deleteTilesAbove; y++){
            if (!isValidPosition(Vector2Int(x, y), config)){
                break;
            }
            if (map(x, y) == 0){
                break;
            }
            map(x, y) = 0;
        }
        for (int y = y1 - 1; y > y1 - config.wallHeightMinimum && deleteTilesBelow; y--){
            if (!isValidPosition(Vector2Int(x, y), config)){
                break;
            }
            if (map(x, y) == 0){
                break;
            }
            map(x, y) = 0;
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

    void enforceMinimumWallHeight(const RandomWalkRoomGeneratorConfig& config, TileMap& map){
        for (int x = config.leftPadding; x < config.mapWidth - config.rightPadding; x++){
            int count = 0;
            for (int y = config.topPadding; y < config.mapHeight - config.bottomPadding; y++){
                if (map(x, y) == 1){
                    count++;
                }
                if (map(x, y) == 0){
                    if (count < config.wallWidthMinimum){
                        for (int yd = y - count; yd < y; yd++){
                            map(x, yd) = 0;
                        }
                    }
                    count = 0;
                }
            }
        }
    }

    void enforceMinimumWallWidth(const RandomWalkRoomGeneratorConfig& config, TileMap& map){
        for (int y = config.bottomPadding; y < config.mapHeight - config.topPadding; y++){
            int count = 0;
            for (int x = config.leftPadding; x < config.mapWidth - config.rightPadding; x++){
                if (map(x, y) == 1){
                    count++;
                }
                if (map(x, y) == 0){
                    if (count < config.wallWidthMinimum){
                        for (int xd = x - count; xd < x; xd++){
                            map(xd, y) = 0;
                        }
                    }
                    count = 0;
                }
            }
        }
    }

    bool brushCanBeApplied(Vector2Int pos, const RandomWalkRoomGeneratorConfig& config){
        for (int x = pos.x - config.brushSize / 2; x < pos.x - config.brushSize / 2 + config.brushSize; x++){
            for (int y = pos.y - config.brushSize / 2; y < pos.y - config.brushSize / 2 + config.brushSize; y++){
                if (!isValidPosition(Vector2Int(x, y), config)){
                    return false;
                }
            }
        }
        return true;
    }

    bool isValidPosition(Vector2Int pos, const RandomWalkRoomGeneratorConfig& config){
        return pos.x >= config.leftPadding && pos.x < config.mapWidth - config.rightPadding && pos.y >= config.topPadding && pos.y < config.mapHeight - config.bottomPadding;
    }
}