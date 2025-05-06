#include <gtest/gtest.h>
#include <complex>

#include "core/plane_field.hpp"
#include "core/transform.hpp"

namespace {
constexpr auto kDefaultSize = 3;
constexpr std::complex<double> kDefaultRawData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

void VerifySquaredValues(diffraction::PlaneField& field) {
    auto expectedValue = 1;

    for (auto y = 0; y < kDefaultSize; ++y) {
        for (auto x = 0; x < kDefaultSize; ++x) {
            const double expected = static_cast<double>(expectedValue * expectedValue);
            EXPECT_DOUBLE_EQ(field[y][x].real(), expected);
            ++expectedValue;
        }
    }
}
}  // namespace

TEST(MultiplyTransformerTest, SameSizeMultiplication) {
    diffraction::PlaneField input(kDefaultSize, kDefaultSize, kDefaultRawData);
    diffraction::PlaneField multiplier(kDefaultSize, kDefaultSize, kDefaultRawData);

    diffraction::MultiplyTransformer transformer(multiplier);
    transformer.transform(input);

    VerifySquaredValues(input);
}

TEST(MultiplyTransformerTest, ComplexValuesMultiplication) {
    diffraction::PlaneField input(1, 2);
    input[0][0] = {1.0, 2.0};
    input[0][1] = {3.0, 4.0};

    diffraction::PlaneField multiplier(1, 2);
    multiplier[0][0] = {2.0, 0.0};
    multiplier[0][1] = {0.0, 1.0};

    diffraction::MultiplyTransformer transformer(multiplier);
    transformer.transform(input);

    EXPECT_DOUBLE_EQ(input[0][0].real(), 2.0);
    EXPECT_DOUBLE_EQ(input[0][0].imag(), 4.0);
    EXPECT_DOUBLE_EQ(input[0][1].real(), -4.0);
    EXPECT_DOUBLE_EQ(input[0][1].imag(), 3.0);
}

TEST(NormTransformerTest, RealValues) {
    diffraction::PlaneField input(kDefaultSize, kDefaultSize, kDefaultRawData);

    diffraction::NormTransformer transformer;
    transformer.transform(input);

    VerifySquaredValues(input);
}

TEST(NormTransformerTest, ComplexValues) {
    diffraction::PlaneField input(1, 2);
    input[0][0] = {1.0, 1.0};
    input[0][1] = {2.0, 3.0};

    diffraction::NormTransformer transformer;
    transformer.transform(input);

    EXPECT_DOUBLE_EQ(input[0][0].real(), 2.0);
    EXPECT_DOUBLE_EQ(input[0][1].real(), 13.0);
}

TEST(FillTransformerTest, FillValues) {
    diffraction::PlaneField input(kDefaultSize, kDefaultSize + 1);

    diffraction::FieldValue fillValue(1.0, -1.0);
    diffraction::FillTransformer transformer(fillValue);
    transformer.transform(input);

    for (auto y = 0; y < kDefaultSize + 1; ++y) {
        for (auto x = 0; x < kDefaultSize; ++x) {
            EXPECT_DOUBLE_EQ(input[y][x].real(), 1.0);
            EXPECT_DOUBLE_EQ(input[y][x].imag(), -1.0);
        }
    }
}

TEST(CoordinatesTransformerTest, MultiplyByCoordinates) {
    diffraction::PlaneField input(2, 2);

    diffraction::FillTransformer fillTransformer(1.0);
    fillTransformer.transform(input);

    auto lambda = [](auto x, auto y, diffraction::FieldValue& value) { value *= (x + y + 1); };
    diffraction::CoordinatesTransformer<decltype(lambda)> lambdaTransformer(lambda);

    lambdaTransformer.transform(input);

    EXPECT_DOUBLE_EQ(input[0][0].real(), 1.0);
    EXPECT_DOUBLE_EQ(input[0][1].real(), 2.0);
    EXPECT_DOUBLE_EQ(input[1][0].real(), 2.0);
    EXPECT_DOUBLE_EQ(input[1][1].real(), 3.0);
}
