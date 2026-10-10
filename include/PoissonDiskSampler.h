#pragma once

#include "Vector2.h"
#include<vector>
#include<utility>

class PoissonDiskSampler {
    public:
        /// Generates a set of points using Poisson Disk Sampling modified to define distance as either x or y direction.
        /// @param width The width of the area to sample.
        /// @param height The height of the area to sample.
        /// @param minDistance The minimum distance between points.
        /// @param points The maximum number of points to generate.
        /// @param maxAttempts The maximum number of attempts to place a point before giving up.
        /// @param seed The seed for the random number generator to ensure reproducibility.
        /// @return A vector of points represented as Vector2.
        std::vector<Vector2> generate(float width, float height, float minDistance, int points, int maxAttempts, int seed);
};