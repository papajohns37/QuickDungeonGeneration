#pragma once

#include <vector>

template <typename T>
struct Grid {
    int width;
    int height;
    std::vector<T> data;

    Grid(int width, int height) : width(width), height(height), data(width * height) {}
    Grid(int width, int height, T defaultValue) : width(width), height(height), data(width * height, defaultValue) {}

    inline T& operator()(int x, int y) {
        return data[y * width + x];
    }

    inline const T& operator()(int x, int y) const {
        return data[y * width + x];
    }

    T* getRow(int y) {
        return data.data() + y * width;
    }
};