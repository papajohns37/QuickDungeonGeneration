#include "PoissonDiskSampler.h"

#include "Grid.h"
#include "Vector2.h"
#include <cmath>
#include <numbers>
#include <random>
#include <stdexcept>

namespace {
    void addSample(Vector2 sample, std::vector<Vector2>& samples, std::vector<int>& active, Grid& grid, float cell);
    bool isValid(Vector2 v, const Grid& grid, const std::vector<Vector2>& samples, float minDist, float cellSize);
}

std::vector<Vector2> PoissonDiskSampler::generate(float width, float height, float minDistance, int points, int maxAttempts, int seed) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Width and height must be positive.");
    }
    if (minDistance <= 0) {
        throw std::invalid_argument("Minimum distance must be positive.");
    }
    if (points <= 0) {
        throw std::invalid_argument("Number of points must be positive.");
    }
    if (maxAttempts <= 0) {
        throw std::invalid_argument("Maximum attempts must be positive.");
    }
    if (width != width || height != height) {
        throw std::invalid_argument("Width and height must be real numbers.");
    }
    if (!std::isfinite(width) ||
        !std::isfinite(height) ||
        !std::isfinite(minDistance)) {
        throw std::invalid_argument("Dimensions and minimum distance must be finite.");
    }

    float cellSize = minDistance / 2.0f;
    if (cellSize <= 0) {
        throw std::invalid_argument("Cell size must be positive.");
    }
    if (cellSize != cellSize || !std::isfinite(cellSize)) {
        throw std::invalid_argument("Cell size must be a finite real number.");
    }
    if (cellSize < std::numeric_limits<float>::epsilon()) {
        throw std::invalid_argument("Cell size cannot be converted to an int.");
    }
    int gridWidth = static_cast<int>(std::ceil(width / cellSize));
    int gridHeight = static_cast<int>(std::ceil(height / cellSize));
    Grid grid(gridWidth, gridHeight, -1);
    std::vector<Vector2> samples;
    std::vector<int> active;
    addSample(Vector2(width / 2, height / 2), samples, active, grid, cellSize);

    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> angleGen(0, 2 * std::numbers::pi);
    std::uniform_real_distribution<float> distGen(cellSize, cellSize * 2);

    while (active.size() > 0 && samples.size() < points){
        std::uniform_int_distribution<int> nextElement(0, active.size() - 1);
        int index = nextElement(rng);
        Vector2 parent = samples[active[index]];
        bool found = false;

        for (int n = 0; n < maxAttempts; n++){
            float angle = angleGen(rng);
            float distance = distGen(rng);
            Vector2 candidate(parent.x + distance * std::cos(angle), parent.y + distance * std::sin(angle));

            if (candidate.x < 0 || candidate.y < 0 || candidate.x >= width || candidate.y >= height){
                continue;
            }
            if (!isValid(candidate, grid, samples, minDistance, cellSize)){
                continue;
            }

            addSample(candidate, samples, active, grid, cellSize);
            found = true;
            break;
        }
        if (!found){
            active.erase(active.begin() + index);
        }
    }

    return samples;
}

namespace {
    void addSample(Vector2 sample, std::vector<Vector2>& samples, std::vector<int>& active, Grid& grid, float cell) {
        int i = static_cast<int>(sample.x / cell);
        int j = static_cast<int>(sample.y / cell);
        grid(i, j) = samples.size();
        samples.push_back(sample);
        active.push_back(samples.size() - 1);
    }

    bool isValid(Vector2 v, const Grid& grid, const std::vector<Vector2>& samples, float minDist, float cellSize){
        int gi = static_cast<int>(v.x / cellSize);
        int gj = static_cast<int>(v.y / cellSize);
        for (int di = -1; di <= 1; di++){
            for (int dj = -1; dj <= 1; dj++){
                int ci = gi + di;
                int cj = gj + dj;
                if (ci < 0 || ci >= grid.width || cj < 0 || cj >= grid.height){
                    continue;
                }
                int sIdx = grid(ci, cj);
                if (sIdx == -1){
                    continue;
                }
                float dx = samples[sIdx].x - v.x;
                float dy = samples[sIdx].y - v.y;
                if (std::abs(dx) < minDist / 2.0f &&
                    std::abs(dy) < minDist / 2.0f) {
                    return false;
                }
            }
        }
        return true;
    }
}