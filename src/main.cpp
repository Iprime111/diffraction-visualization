#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "conversions/wavelength_lookup.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"
#include "fmt/base.h"

constexpr auto kWindowWidth = 800;
constexpr auto kWindowHeight = 600;
constexpr auto kPixelComponentsCount = 4; // RGBA
                                       
// TODO remove
constexpr auto kUsedWavelength = 430.0;

namespace {
void getIntensityRgbData(diffraction::MonochromaticField& field, std::vector<diffraction::RGBData>& rgbData);
void fillTextureWithRgb( std::vector<diffraction::RGBData>& rgbData, std::vector<std::uint8_t>& texturePixels);
} // namespace

int main() {
    // TODO handle exceptions

    sf::RenderWindow window{sf::VideoMode({kWindowWidth, kWindowHeight}), 
                      "Diffraction", sf::Style::Close, sf::State::Windowed};

    sf::Texture texture{sf::Vector2u{kWindowWidth, kWindowHeight}};

    std::vector<std::uint8_t> texturePixels(kWindowWidth * kWindowHeight * kPixelComponentsCount);
    std::vector<diffraction::RGBData> rgbData(kWindowWidth * kWindowHeight);

    std::fill(texturePixels.begin(), texturePixels.end(), 255); // Paint default texture white to enable alpha channel
    texture.update(texturePixels.data());

    sf::Sprite textureSprite{texture};

    diffraction::MonochromaticField resultField{kWindowWidth, kWindowHeight, kUsedWavelength};
    for (auto& value : resultField) {
        value = 1.0;
    }

    getIntensityRgbData(resultField, rgbData);
    fillTextureWithRgb(rgbData, texturePixels);
    texture.update(texturePixels.data());

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
    
        // TODO perform actual simulation


        window.draw(textureSprite);
        window.display();
    }
}

namespace {
void getIntensityRgbData(diffraction::MonochromaticField& field, std::vector<diffraction::RGBData>& rgbData) {
    fmt::println("Before norm: {}", field[0][0].real());
    diffraction::Transformable{field}.transform(diffraction::NormTransformer{});
    fmt::println("After norm: {}", field[0][0].real());

    auto fieldSize = field.getXSize() * field.getYSize();
    auto fieldIt = field.begin();
    auto wavelength = field.getWavelength();

    for (auto valueIdx = 0; valueIdx < fieldSize; ++valueIdx, ++fieldIt) {
        rgbData[valueIdx] = diffraction::wavelengthToRgb(wavelength, fieldIt->real());
    }

    diffraction::Transformable{rgbData}.transform(diffraction::RgbNormTransform{});
}

void fillTextureWithRgb( std::vector<diffraction::RGBData>& rgbData, std::vector<std::uint8_t>& texturePixels) {

    for (auto pixelIdx = 0; pixelIdx < kWindowWidth * kWindowHeight; ++pixelIdx) {
        constexpr auto kMaxChannelValue = 255;

        auto pixelComponentIdx = pixelIdx * kPixelComponentsCount;

        texturePixels[pixelComponentIdx] = rgbData[pixelIdx].r * kMaxChannelValue;
        texturePixels[pixelComponentIdx + 1] = rgbData[pixelIdx].g * kMaxChannelValue;
        texturePixels[pixelComponentIdx + 2] = rgbData[pixelIdx].b * kMaxChannelValue;
    }
}
} // namespace
