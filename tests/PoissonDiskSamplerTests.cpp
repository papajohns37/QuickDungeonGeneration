#include "PoissonDiskSampler.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
    void require(bool condition, const char* message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    template <typename Callable>
    void requireInvalidArgument(Callable&& callable, const char* message) {
        try {
            callable();
        } catch (const std::invalid_argument&) {
            return;
        }

        throw std::runtime_error(message);
    }

    bool isOutsideExclusionSquare(const Vector2& first,
                                  const Vector2& second,
                                  float sideLength) {
        return std::abs(first.x - second.x) >= sideLength / 2.0f ||
               std::abs(first.y - second.y) >= sideLength / 2.0f;
    }

    void testPointsStayInsideBounds() {
        constexpr float width = 40.0f;
        constexpr float height = 25.0f;
        PoissonDiskSampler sampler;

        const auto samples = sampler.generate(width, height, 3.0f, 20, 30, 1234);

        for (const auto& sample : samples) {
            require(sample.x >= 0.0f && sample.x < width &&
                        sample.y >= 0.0f && sample.y < height,
                    "A generated point was outside the sampling bounds.");
        }
    }

    void testPointsRespectMinimumSquareSeparation() {
        constexpr float minDistance = 3.0f;
        PoissonDiskSampler sampler;
        const auto samples = sampler.generate(40.0f, 25.0f, minDistance, 30, 30, 1234);

        for (std::size_t first = 0; first < samples.size(); ++first) {
            for (std::size_t second = first + 1; second < samples.size(); ++second) {
                require(isOutsideExclusionSquare(samples[first], samples[second],
                                                 minDistance),
                        "Two generated points are inside the same exclusion square.");
            }
        }
    }

    void testGeneratedPointsAreFiniteAndUnique() {
        PoissonDiskSampler sampler;
        const auto samples = sampler.generate(40.0f, 25.0f, 3.0f, 50, 30, 1234);

        for (std::size_t first = 0; first < samples.size(); ++first) {
            require(std::isfinite(samples[first].x) &&
                        std::isfinite(samples[first].y),
                    "The sampler returned a non-finite point.");
            for (std::size_t second = first + 1; second < samples.size(); ++second) {
                require(samples[first].x != samples[second].x ||
                            samples[first].y != samples[second].y,
                        "The sampler returned duplicate points.");
            }
        }
    }

    void testSameSeedIsDeterministic() {
        PoissonDiskSampler sampler;

        const auto first = sampler.generate(40.0f, 25.0f, 3.0f, 30, 30, 9876);
        const auto second = sampler.generate(40.0f, 25.0f, 3.0f, 30, 30, 9876);

        require(first.size() == second.size(),
                "The same seed produced different sample counts.");
        for (std::size_t index = 0; index < first.size(); ++index) {
            require(first[index].x == second[index].x &&
                        first[index].y == second[index].y,
                    "The same seed did not produce the same samples.");
        }
    }

    void testRequestedPointCountIsAnUpperBound() {
        constexpr int requestedPoints = 20;
        PoissonDiskSampler sampler;
        const auto samples = sampler.generate(40.0f, 25.0f, 3.0f,
                                              requestedPoints, 30, 1234);

        require(samples.size() <= static_cast<std::size_t>(requestedPoints),
                "The sampler returned more points than requested.");
    }

    void testOneRequestedPointReturnsTheInitialPoint() {
        PoissonDiskSampler sampler;
        const auto samples = sampler.generate(40.0f, 25.0f, 3.0f, 1, 30, 1234);

        require(samples.size() == 1,
                "Requesting one point should return exactly one point.");
        require(samples.front().x == 20.0f && samples.front().y == 12.5f,
                "The initial point was not placed at the center.");
    }

    void testImpossibleLayoutReturnsTheInitialPoint() {
        PoissonDiskSampler sampler;
        const auto samples = sampler.generate(1.0f, 1.0f, 3.0f, 20, 30, 1234);

        require(samples.size() == 1,
                "A layout that cannot fit additional points should return only the initial point.");
        require(samples.front().x == 0.5f && samples.front().y == 0.5f,
                "The initial point was not centered in a small layout.");
    }

    void testMinimumAttemptBudgetStillProducesValidOutput() {
        constexpr float minDistance = 3.0f;
        PoissonDiskSampler sampler;
        const auto samples = sampler.generate(40.0f, 25.0f, minDistance, 20, 1, 1234);

        require(!samples.empty(),
                "The sampler returned no points with one attempt per active point.");
        for (std::size_t first = 0; first < samples.size(); ++first) {
            for (std::size_t second = first + 1; second < samples.size(); ++second) {
                require(isOutsideExclusionSquare(samples[first], samples[second],
                                                 minDistance),
                        "A low-attempt run violated the minimum square separation.");
            }
        }
    }

    void testInvalidArgumentsAreRejected() {
        PoissonDiskSampler sampler;

        requireInvalidArgument([&] {
            sampler.generate(0.0f, 25.0f, 3.0f, 20, 30, 1234);
        }, "A non-positive width was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, 0.0f, 3.0f, 20, 30, 1234);
        }, "A non-positive height was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, 25.0f, 0.0f, 20, 30, 1234);
        }, "A non-positive minimum distance was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, 25.0f, 3.0f, 0, 30, 1234);
        }, "A non-positive point count was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, 25.0f, 3.0f, 20, 0, 1234);
        }, "A non-positive attempt count was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(NAN, 25.0f, 3.0f, 20, 30, 1234);
        }, "A NaN width was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(-INFINITY, 25.0f, 3.0f, 20, 30, 1234);
        }, "A negative infinite width was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, INFINITY, 3.0f, 20, 30, 1234);
        }, "An infinite height was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, 25.0f, NAN, 20, 30, 1234);
        }, "A NaN minimum distance was accepted.");
        requireInvalidArgument([&] {
            sampler.generate(40.0f, 25.0f, INFINITY, 20, 30, 1234);
        }, "An infinite minimum distance was accepted.");
    }
}

int main() {
    try {
        testPointsStayInsideBounds();
        testPointsRespectMinimumSquareSeparation();
        testGeneratedPointsAreFiniteAndUnique();
        testSameSeedIsDeterministic();
        testRequestedPointCountIsAnUpperBound();
        testOneRequestedPointReturnsTheInitialPoint();
        testImpossibleLayoutReturnsTheInitialPoint();
        testMinimumAttemptBudgetStillProducesValidOutput();
        testInvalidArgumentsAreRejected();
    } catch (const std::exception& error) {
        std::cerr << "PoissonDiskSamplerTests failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
