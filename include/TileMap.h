#include <vector>
#include <cstdint>

struct TileMap {
    int width;
    int height;
    std::vector<uint8_t> data;

    TileMap(int width, int height) : width(width), height(height), data(width * height, 0) {}

    inline uint8_t& operator()(int x, int y) {
        return data[y * width + x];
    }

    inline const uint8_t& operator()(int x, int y) const {
        return data[y * width + x];
    }

    uint8_t* getRow(int y) {
        return data.data() + y * width;
    }
};