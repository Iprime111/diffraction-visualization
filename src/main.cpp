#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <array>
#include <cstdint>

#include "core/plane_field.hpp"
#include "core/transform.hpp"

constexpr auto kWindowWidth = 800;
constexpr auto kWindowHeight = 600;
constexpr auto kPixelComponentsCount = 4; // RGBA

namespace {
void fillTextureWithIntensity(diffraction::MonochromaticField& field, std::vector<std::uint8_t>& texturePixels);
std::array<std::uint8_t, 3> wavelengthToRgb(double wavelength, double intensity);
} // namespace

int main() {
    // TODO handle exceptions

    sf::RenderWindow window{sf::VideoMode({kWindowWidth, kWindowHeight}), 
                      "Diffraction", sf::Style::Close, sf::State::Windowed};

    sf::Texture texture{sf::Vector2u{kWindowWidth, kWindowHeight}};
    std::vector<std::uint8_t> texturePixels(kWindowWidth * kWindowHeight * kPixelComponentsCount);

    std::fill(texturePixels.begin(), texturePixels.end(), 255); // Paint default texture white to enable alpha channel
    texture.update(texturePixels.data());

    sf::Sprite textureSprite{texture};

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.draw(textureSprite);
        window.display();
    }

}

namespace {
void fillTextureWithIntensity(diffraction::MonochromaticField& field, std::vector<std::uint8_t>& texturePixels) {
    diffraction::Transformable{field}.transform(diffraction::NormTransformer{});

    for (std::size_t y = 0; y < kWindowHeight; ++y) {
        for (std::size_t x = 0; x < kWindowWidth; ++x) {

            std::size_t pixelIdx = (y * kWindowWidth + x) * kPixelComponentsCount;
            auto rgbValues = wavelengthToRgb(field.getWavelength(), field[x][y].real());


            texturePixels[pixelIdx] = rgbValues[0];
            texturePixels[pixelIdx + 1] = rgbValues[1];
            texturePixels[pixelIdx + 2] = rgbValues[2];
        }
    }
}

std::array<std::uint8_t, 3> wavelengthToRgb(double wavelength, double intensity) {
    // TODO
    return std::array<std::uint8_t, 3>{};
}
} // namespace
