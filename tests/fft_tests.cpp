#include <cstddef>
#include <gtest/gtest.h>
#include <fftw3.h>
#include <complex>
#include <vector>
#include <fmt/format.h>

#include "core/fft.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"
#include "fmt/base.h"

namespace {
constexpr double kTolerance = 1e-6;
constexpr std::size_t kSize = 4;
}

TEST(FFT2DTest, ForwardTransform) {
    diffraction::PlaneField input(kSize, kSize);

    for (std::size_t y = 0; y < kSize; ++y) {
        for (std::size_t x = 0; x < kSize; ++x) {
            input[y][x] = {(y == 0 && x == 0) ? 1.0 : 0.0, 0.0};
        }
    }

    diffraction::FFT2D fft;
    fft.transform(input);

    for (std::size_t y = 0; y < kSize; ++y) {
        for (std::size_t x = 0; x < kSize; ++x) {
            EXPECT_NEAR(1.0, input[y][x].real(), kTolerance);
            EXPECT_NEAR(0.0, input[y][x].imag(), kTolerance);
        }
    }
}

TEST(FFT2DTest, InverseTransform) {
    diffraction::PlaneField input(kSize, kSize);

    for (std::size_t y = 0; y < kSize; ++y) {
        for (std::size_t x = 0; x < kSize; ++x) {
            input[y][x] = {1.0, 0.0};
        }
    }

    diffraction::FFT2DInverse fftInverse;
    fftInverse.transform(input);

    const double expected_value = kSize * kSize;
    
    for (std::size_t y = 0; y < kSize; ++y) {
        for (std::size_t x = 0; x < kSize; ++x) {
            if (y == 0 && x == 0) {
                EXPECT_NEAR(expected_value, input[y][x].real(), kTolerance);
            } else {
                EXPECT_NEAR(0.0, input[y][x].real(), kTolerance);
            }
            EXPECT_NEAR(0.0, input[y][x].imag(), kTolerance);
        }
    }
}    