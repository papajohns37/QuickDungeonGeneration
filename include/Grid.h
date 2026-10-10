#pragma once

#include <vector>

struct Grid {
    int width;
    int height;
    std::vector<int> data;

    Grid(int width, int height) : width(width), height(height), data(width * height, 0) {}
    Grid(int width, int height, int defaultValue) : width(width), height(height), data(width * height, defaultValue) {}

    inline int& operator()(int x, int y) {
        return data[y * width + x];
    }

    inline const int& operator()(int x, int y) const {
        return data[y * width + x];
    }

    int* getRow(int y) {
        return data.data() + y * width;
    }
};