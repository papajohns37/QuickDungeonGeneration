#include <RandomWalkGenerator.h>
#include <random>

void replaceWithFloor(Vector2 position, int brushSize, int wallHeightMinimum, int mapHeight, int mapWidth, int** map);
void deleteTile(int x, int y1, int wallHeightMinimum, int mapHeight, int mapWidth, int** map);
Vector2 positionPlusDirection(Vector2 startPosition, int direction);
bool isValidPosition(Vector2 pos, int mapWidth, int mapHeight);

//TODO: Add padding
void RandomWalkGenerator::generate(int mapWidth, int mapHeight, int numberOfSteps, int brushSize, int wallHeightMinimum, Vector2 startPosition, int** map, int seed) {
    // Implementation for generating the random walk

    // Populate the wap with walls
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            map[y][x] = 1;
        }
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> directionDist(0, 3);

    for (int i = 0; i < numberOfSteps; i++){
        int direction;
        do {
            direction = directionDist(rng);
        } while (!isValidPosition(positionPlusDirection(startPosition, direction), mapWidth, mapHeight));

        startPosition = positionPlusDirection(startPosition, direction);

        replaceWithFloor(startPosition, brushSize, wallHeightMinimum, mapHeight, mapWidth, map);
    }
}

void replaceWithFloor(Vector2 position, int brushSize, int wallHeightMinimum, int mapHeight, int mapWidth, int** map){
    for (int x = static_cast<int>(position.x) - brushSize / 2; x < static_cast<int>(position.x) - brushSize / 2 + brushSize; x++){
        for (int y = static_cast<int>(position.y) - brushSize / 2; y < static_cast<int>(position.y) - brushSize / 2 + brushSize; y++){
            deleteTile(x, y, wallHeightMinimum, mapHeight, mapWidth, map);
        }
    }
}

void deleteTile(int x, int y1, int wallHeightMinimum, int mapHeight, int mapWidth, int** map){
    if (!isValidPosition(Vector2(x, y1), mapWidth, mapHeight)){
        return;
    }
    map[y1][x] = 0;

    //Check if above violates wall height minimum
    bool deleteTilesAbove = false;
    for (int y = y1 + 1; y < y1 + wallHeightMinimum; y++){
        if (!isValidPosition(Vector2(x, y + 1), mapWidth, mapHeight)){
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
    for (int y = y1 - 1; y > y1 - wallHeightMinimum; y--){
        if (!isValidPosition(Vector2(x, y - 1), mapWidth, mapHeight)){
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
    for (int y = y1 + 1; y < y1 + wallHeightMinimum && deleteTilesAbove; y++){
        if (!isValidPosition(Vector2(x, y), mapWidth, mapHeight)){
            break;
        }
        if (map[y][x] == 0){
            break;
        }
        map[y][x] = 0;
    }
    for (int y = y1 - 1; y > y1 - wallHeightMinimum && deleteTilesBelow; y--){
        if (!isValidPosition(Vector2(x, y), mapWidth, mapHeight)){
            break;
        }
        if (map[y][x] == 0){
            break;
        }
        map[y][x] = 0;
    }
}

Vector2 positionPlusDirection(Vector2 startPosition, int direction){
    switch (direction){ 
        case 0: return Vector2(startPosition.x, startPosition.y + 1);
        case 1: return Vector2(startPosition.x, startPosition.y - 1);
        case 2: return Vector2(startPosition.x + 1, startPosition.y);
        case 3: return Vector2(startPosition.x - 1, startPosition.y);
    }

    return startPosition; // Should never reach here
}

bool isValidPosition(Vector2 pos, int mapWidth, int mapHeight) {
    return (pos.x >= 0 && pos.x < mapWidth && pos.y >= 0 && pos.y < mapHeight);
}
