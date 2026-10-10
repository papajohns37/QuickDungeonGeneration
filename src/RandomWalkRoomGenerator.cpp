#include "RandomWalkRoomGenerator.h"
#include "WallThicknessEnforcer.h"
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

namespace {
    void applyBrush(Vector2Int position, const RandomWalkRoomGeneratorConfig& config, TileMap& map);
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

    WallThicknessEnforcer wallThicknessEnforcer;
    wallThicknessEnforcer.enforceWallThickness(map, config);
}


namespace {
    void applyBrush(Vector2Int position, const RandomWalkRoomGeneratorConfig& config, TileMap& map){
        if (!brushCanBeApplied(position, config)){
            return;
        }
        for (int x = position.x - config.brushSize / 2; x < position.x - config.brushSize / 2 + config.brushSize; x++){
            for (int y = position.y - config.brushSize / 2; y < position.y - config.brushSize / 2 + config.brushSize; y++){
                map(x, y) = config.floorValue;
            }
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