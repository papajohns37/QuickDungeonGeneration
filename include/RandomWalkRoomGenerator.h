#pragma once

#include "RandomWalkRoomGeneratorConfig.h"
#include "TileMap.h"
#include "Vector2Int.h"

class RandomWalkRoomGenerator {
    public:
        /// Generates a random walk on a 2D map.
        /// @param mapWidth The width of the map.
        /// @param mapHeight The height of the map.
        /// @param numberOfSteps The number of steps to take in the random walk, an additional step is taken to clear starting position.
        /// @param brushSize The size of the brush used to carve out the walk.
        /// Brush size just determines what is carved, some edge cases will create smaller areas of floor.
        /// @param wallHeightMinimum The minimum height of walls that should be preserved.
        /// @param xPadding The padding on the x-axis to prevent carving too close to the
        /// edges of the map.
        /// @param yPadding The padding on the y-axis to prevent carving too close to the
        /// edges of the map.
        /// @param startPosition The starting position of the random walk.
        /// @param map The 2D array representing the map, where 1 represents a wall and 0 represents a floor.
        /// The map should be pre-allocated with the specified width and height.
        /// The function will modify this map in place to create the random walk.
        /// @param seed The seed for the random number generator to ensure reproducibility.
        void generate(RandomWalkRoomGeneratorConfig config, TileMap& map, int seed);
};