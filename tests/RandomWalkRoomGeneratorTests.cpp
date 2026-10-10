#include "RandomWalkRoomGenerator.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
    class TestMap {
    public:
        TestMap(int width, int height)
            : map_(width, height, 1) {
        }

        TileMap& data() {
            return map_;
        }

        int at(int x, int y) const {
            return map_(x, y);
        }

        bool operator==(const TestMap& other) const {
            return map_.data == other.map_.data;
        }

        int width() const {
            return map_.width;
        }

        int height() const {
            return map_.height;
        }

    private:
        TileMap map_;
    };

    RandomWalkRoomGeneratorConfig defaultConfig() {
        return {
            40,
            40,
            200,
            2,
            3,
            3,
            3,
            3,
            3,
            3,
            0,
            {20, 20}
        };
    }

    void require(bool condition, const std::string& message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    void requireInvalid(RandomWalkRoomGeneratorConfig config, TileMap& map, const std::string& message) {
        RandomWalkRoomGenerator generator;
        try {
            generator.generate(config, map, 1);
        } catch (const std::invalid_argument&) {
            return;
        }
        throw std::runtime_error(message);
    }

    void requireWallHeightInvariant(const TestMap& map, int minimumHeight) {
        for (int x = 0; x < map.width(); ++x) {
            int y = 0;
            while (y < map.height()) {
                if (map.at(x, y) == 0) {
                    ++y;
                    continue;
                }

                const int start = y;
                while (y < map.height() && map.at(x, y) == 1) {
                    ++y;
                }
                require(y - start >= minimumHeight,
                        "A wall run is shorter than the configured minimum height.");
            }
        }
    }

    void testGeneratesFloorsInsidePadding() {
        const auto config = defaultConfig();
        TestMap map(config.mapWidth, config.mapHeight);
        RandomWalkRoomGenerator generator;

        generator.generate(config, map.data(), 1234);

        int floorCount = 0;
        for (int y = 0; y < map.height(); ++y) {
            for (int x = 0; x < map.width(); ++x) {
                require(map.at(x, y) == 0 || map.at(x, y) == 1, "Map contains an invalid tile value.");
                if (map.at(x, y) == 0) {
                    ++floorCount;
                    require(x >= config.leftPadding &&
                            x < config.mapWidth - config.rightPadding &&
                            y >= config.topPadding &&
                            y < config.mapHeight - config.bottomPadding,
                            "Floor was carved outside the configured padding.");
                }
            }
        }
        require(floorCount > 0, "Generation did not carve any floor tiles.");
    }

    void testRectangularMaps() {
        RandomWalkRoomGenerator generator;
        const std::vector<std::pair<int, int>> dimensions = {
            {80, 40},
            {40, 80},
            {17, 35}
        };

        for (const auto [width, height] : dimensions) {
            auto config = defaultConfig();
            config.mapWidth = width;
            config.mapHeight = height;
            config.startPosition = {width / 2, height / 2};
            TestMap map(width, height);

            generator.generate(config, map.data(), 1234);

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    require(map.at(x, y) == 0 || map.at(x, y) == 1,
                            "Rectangular map contains an invalid tile value.");
                }
            }
        }
    }

    void testSeedIsDeterministic() {
        const auto config = defaultConfig();
        TestMap first(config.mapWidth, config.mapHeight);
        TestMap second(config.mapWidth, config.mapHeight);
        RandomWalkRoomGenerator generator;

        generator.generate(config, first.data(), 9876);
        generator.generate(config, second.data(), 9876);

        require(first == second, "The same seed did not produce the same map.");
    }

    void testBrushSizes() {
        RandomWalkRoomGenerator generator;
        for (const int brushSize : {1, 2, 3, 4}) {
            auto config = defaultConfig();
            config.brushSize = brushSize;
            TestMap map(config.mapWidth, config.mapHeight);

            generator.generate(config, map.data(), 1234);

            for (int y = 0; y < map.height(); ++y) {
                for (int x = 0; x < map.width(); ++x) {
                    require(map.at(x, y) == 0 || map.at(x, y) == 1,
                            "Brush-size test produced an invalid tile value.");
                }
            }
        }
    }

    void testWallHeightMinimumAcrossSeeds() {
        const auto config = defaultConfig();
        RandomWalkRoomGenerator generator;

        for (int seed = 0; seed < 100; ++seed) {
            TestMap map(config.mapWidth, config.mapHeight);
            generator.generate(config, map.data(), seed);
            requireWallHeightInvariant(map, config.wallHeightMinimum);
        }
    }

    void testWallMinimumsRecognizeCustomFloorValues() {
            auto config = defaultConfig();
            config.floorValue = 7;
            config.numberOfSteps = 0;
            config.wallHeightMinimum = 3;
            config.wallWidthMinimum = 3;
            TestMap map(config.mapWidth, config.mapHeight);
            RandomWalkRoomGenerator generator;

            generator.generate(config, map.data(), 1234);

            for (int x = config.leftPadding; x < config.mapWidth - config.rightPadding; ++x) {
                int run = 0;
                for (int y = config.topPadding; y < config.mapHeight - config.bottomPadding; ++y) {
                    if (map.at(x, y) == config.floorValue) {
                        require(run == 0 || run >= config.wallHeightMinimum,
                                "Custom floor values were not recognized during wall-height enforcement.");
                        run = 0;
                    } else {
                        ++run;
                }
            }
                require(run == 0 || run >= config.wallHeightMinimum,
                        "Custom floor values were not recognized during wall-height enforcement.");
            }
    }

    void testZeroStepsCarvesOnlyStartingBrush() {
        auto config = defaultConfig();
        config.numberOfSteps = 0;
        config.brushSize = 1;
        TestMap map(config.mapWidth, config.mapHeight);
        RandomWalkRoomGenerator generator;

        generator.generate(config, map.data(), 1234);

        int floorCount = 0;
        for (int y = 0; y < map.height(); ++y) {
            for (int x = 0; x < map.width(); ++x) {
                floorCount += map.at(x, y) == 0 ? 1 : 0;
            }
        }
        require(floorCount == 1, "Zero steps did not carve exactly the starting tile.");
        require(map.at(config.startPosition.x, config.startPosition.y) == 0,
                "Zero steps did not carve the starting tile.");
    }

    void testBoundaryStartingPositions() {
        auto config = defaultConfig();
        config.brushSize = 1;
        RandomWalkRoomGenerator generator;

        const std::vector<Vector2Int> starts = {
            {config.leftPadding, config.topPadding},
            {config.mapWidth - config.rightPadding - 1, config.topPadding},
            {config.leftPadding, config.mapHeight - config.bottomPadding - 1},
            {config.mapWidth - config.rightPadding - 1, config.mapHeight - config.bottomPadding - 1}
        };

        for (const auto start : starts) {
            config.startPosition = start;
            TestMap map(config.mapWidth, config.mapHeight);
            generator.generate(config, map.data(), 1234);
            require(map.at(start.x, start.y) == 0,
                    "A valid boundary start position was not carved.");
        }
    }

    void testInvalidConfiguration() {
        const auto config = defaultConfig();
        TestMap map(config.mapWidth, config.mapHeight);

        auto invalid = config;
        invalid.mapWidth = 0;
        requireInvalid(invalid, map.data(), "Zero map width was accepted.");

        invalid = config;
        invalid.brushSize = 0;
        requireInvalid(invalid, map.data(), "Zero brush size was accepted.");

        invalid = config;
        invalid.wallWidthMinimum = 0;
        requireInvalid(invalid, map.data(), "Zero wall width minimum was accepted.");

        invalid = config;
        invalid.topPadding = 2;
        requireInvalid(invalid, map.data(), "Insufficient vertical padding was accepted.");

        invalid = config;
        invalid.startPosition = {0, 0};
        requireInvalid(invalid, map.data(), "An invalid start position was accepted.");

        invalid = config;
        invalid.leftPadding = -1;
        requireInvalid(invalid, map.data(), "Negative horizontal padding was accepted.");

        invalid = config;
        invalid.bottomPadding = -1;
        requireInvalid(invalid, map.data(), "Negative vertical padding was accepted.");

        invalid = config;
        invalid.numberOfSteps = -1;
        requireInvalid(invalid, map.data(), "Negative step count was accepted.");

        invalid = config;
        invalid.mapWidth = 4;
        invalid.leftPadding = 1;
        invalid.rightPadding = 1;
        invalid.startPosition = {2, 20};
        requireInvalid(invalid, map.data(), "An effective width smaller than the brush was accepted.");

        invalid = config;
        invalid.mapHeight = 7;
        invalid.topPadding = 3;
        invalid.bottomPadding = 3;
        invalid.startPosition = {20, 3};
        requireInvalid(invalid, map.data(), "An effective height smaller than the brush was accepted.");

        invalid = config;
        TileMap mismatchedMap(config.mapWidth - 1, config.mapHeight);
        requireInvalid(invalid, mismatchedMap, "A map with mismatched dimensions was accepted.");
    }

    void testBrushFootprint() {
        auto config = defaultConfig();
        config.numberOfSteps = 0;
        config.brushSize = 3;
        config.startPosition = {20, 20};
        TestMap map(config.mapWidth, config.mapHeight);
        RandomWalkRoomGenerator generator;

        generator.generate(config, map.data(), 1234);

        int floorCount = 0;
        for (int y = 0; y < map.height(); ++y) {
            for (int x = 0; x < map.width(); ++x) {
                floorCount += map.at(x, y) == 0 ? 1 : 0;
            }
        }
        require(floorCount == 9, "A centered 3x3 brush did not carve the expected footprint.");
    }

    void testEvenBrushFootprintAtPaddingBoundary() {
        auto config = defaultConfig();
        config.numberOfSteps = 0;
        config.brushSize = 2;
        config.startPosition = {
            config.leftPadding + config.brushSize / 2,
            config.topPadding + config.brushSize / 2
        };
        TestMap map(config.mapWidth, config.mapHeight);
        RandomWalkRoomGenerator generator;

        generator.generate(config, map.data(), 1234);

        for (int y = 0; y < map.height(); ++y) {
            for (int x = 0; x < map.width(); ++x) {
                const bool expectedFloor =
                    x >= config.leftPadding &&
                    x < config.leftPadding + config.brushSize &&
                    y >= config.topPadding &&
                    y < config.topPadding + config.brushSize;
                require((map.at(x, y) == 0) == expectedFloor,
                        "A 2x2 brush did not carve its complete boundary footprint.");
            }
        }
    }

    void runAllTests() {
        testGeneratesFloorsInsidePadding();
        testRectangularMaps();
        testSeedIsDeterministic();
        testBrushSizes();
        testWallHeightMinimumAcrossSeeds();
        testWallMinimumsRecognizeCustomFloorValues();
        testZeroStepsCarvesOnlyStartingBrush();
        testBoundaryStartingPositions();
        testInvalidConfiguration();
        testBrushFootprint();
        testEvenBrushFootprintAtPaddingBoundary();
    }
}

int main() {
    try {
        runAllTests();
        std::cout << "All RandomWalkRoomGenerator tests passed.\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "RandomWalkRoomGenerator test failed: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}