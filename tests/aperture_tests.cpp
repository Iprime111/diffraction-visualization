#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Vector2.hpp"

#include "core/aperture.hpp"

TEST(ApertureTest, ThresholdTest) {
    sf::Image testImage(sf::Vector2u(4, 4), sf::Color::White);

    testImage.setPixel(sf::Vector2u(1, 1), sf::Color(230, 230, 230));
    testImage.setPixel(sf::Vector2u(2, 2), sf::Color(200, 200, 200));
    testImage.setPixel(sf::Vector2u(3, 3), sf::Color(50, 50, 50));

    diffraction::Aperture highThresholdAperture(testImage, 4, 4, 0.85);

    EXPECT_DOUBLE_EQ(1.0, highThresholdAperture[0][0].real()) << "white should pass 0.85 threshold";

    EXPECT_DOUBLE_EQ(1.0, highThresholdAperture[1][1].real())
        << "230 intensity(0.9) should pass 0.85 threshold";

    EXPECT_DOUBLE_EQ(0.0, highThresholdAperture[2][2].real())
        << "200 intensity(0.78) should not pass 0.85 threshold";

    EXPECT_DOUBLE_EQ(0.0, highThresholdAperture[3][3].real())
        << "50 intensity(0.19) should not pass 0.85 threshold";

    diffraction::Aperture lowThresholdAperture(testImage, 4, 4, 0.5);

    EXPECT_DOUBLE_EQ(1.0, lowThresholdAperture[2][2].real())
        << "200 intensity(0.78) should pass 0.5 threshold";
}

TEST(ApertureTest, ScaledImageTest) {
    sf::Image image(sf::Vector2u(8, 16), sf::Color::Black);

    for (std::size_t y = 0; y < 16; ++y) {
        for (std::size_t x = 0; x < 4; ++x) {
            image.setPixel(sf::Vector2u(x, y), sf::Color::White);
        }
    }

    diffraction::Aperture aperture(image, 4, 4, 0.5);

    for (std::size_t y = 0; y < 4; ++y) {
        EXPECT_DOUBLE_EQ(1.0, aperture[y][0].real());
        EXPECT_DOUBLE_EQ(1.0, aperture[y][1].real());
        EXPECT_DOUBLE_EQ(0.0, aperture[y][2].real());
        EXPECT_DOUBLE_EQ(0.0, aperture[y][3].real());
    }
}

TEST(ApertureTest, VerticalStripesScalingTest) {
    sf::Image stripedImage(sf::Vector2u(4, 4), sf::Color::Black);

    for (std::size_t y = 0; y < 4; ++y) {
        for (std::size_t x = 0; x < 4; x += 2) {
            stripedImage.setPixel(sf::Vector2u(x, y), sf::Color::White);
        }
    }

    diffraction::Aperture aperture(stripedImage, 4, 4, 0.5);

    for (std::size_t y = 0; y < 4; ++y) {
        EXPECT_DOUBLE_EQ(1.0, aperture[y][0].real());
        EXPECT_DOUBLE_EQ(0.0, aperture[y][1].real());
        EXPECT_DOUBLE_EQ(1.0, aperture[y][2].real());
        EXPECT_DOUBLE_EQ(0.0, aperture[y][3].real());
    }
}
