#include <fmt/base.h>

#include <CLI/CLI.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "conversions/wavelength_lookup.hpp"
#include "core/aperture.hpp"
#include "core/attributes.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"

constexpr auto kWindowWidth = 800;
constexpr auto kWindowHeight = 600;
constexpr auto kPixelComponentsCount = 4;  // RGBA
constexpr auto kWavelengthStep = 5;
constexpr auto kDefaultThreshold = 0.5;

// TODO remove
constexpr auto kUsedWavelength = 430.0;
constexpr auto kMinimumWavelength = 380.0;
constexpr auto kMaximumWavelength = 779.0;

constexpr auto kAppTitle = "Diffraction";

namespace {
void getIntensityRgbData(diffraction::MonochromaticField& field, std::vector<diffraction::RGBData>& rgbData);
void fillTextureWithRgb(std::vector<diffraction::RGBData>& rgbData, std::vector<std::uint8_t>& texturePixels);
bool handleEvents(sf::RenderWindow& window, diffraction::MonochromaticField& field);
}  // namespace

int main(int argc, char** argv) {
    // TODO handle exceptions

    CLI::App app{kAppTitle};

    std::string aperturePath;
    app.add_option("-f,--aperture_file", aperturePath, "Path to input aperture image file")
        ->required()
        ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    sf::Image apertureImage;
    if (!apertureImage.loadFromFile(aperturePath)) {
        DIFFRACTION_CRITICAL("Failed to load aperture image: {}", aperturePath);
        return EXIT_FAILURE;
    }

    sf::RenderWindow window{sf::VideoMode({kWindowWidth, kWindowHeight}), kAppTitle, sf::Style::Close,
                            sf::State::Windowed};

    sf::Texture texture{sf::Vector2u{kWindowWidth, kWindowHeight}};

    std::vector<std::uint8_t> texturePixels(kWindowWidth * kWindowHeight * kPixelComponentsCount);
    std::vector<diffraction::RGBData> rgbData(kWindowWidth * kWindowHeight);

    std::fill(texturePixels.begin(), texturePixels.end(),
              255);  // Paint default texture white to enable alpha channel
    texture.update(texturePixels.data());

    sf::Sprite textureSprite{texture};

    diffraction::MonochromaticField resultField{kWindowWidth, kWindowHeight, kUsedWavelength};
    // TODO perform actual simulation
    for (auto& value : resultField) {
        value = 1.0;
    }

    diffraction::Aperture aperture(apertureImage, resultField.getXSize(), resultField.getYSize(),
                                   kDefaultThreshold);

    while (window.isOpen()) {
        if (!handleEvents(window, resultField)) {
            window.close();
            break;
        }

        getIntensityRgbData(resultField, rgbData);
        fillTextureWithRgb(rgbData, texturePixels);
        texture.update(texturePixels.data());

        window.draw(textureSprite);
        window.display();
    }

    return 0;
}

namespace {
void getIntensityRgbData(diffraction::MonochromaticField& field, std::vector<diffraction::RGBData>& rgbData) {
    diffraction::Transformable{field}.transform(diffraction::NormTransformer{});

    auto fieldSize = field.getXSize() * field.getYSize();
    auto fieldIt = field.begin();
    auto wavelength = field.getWavelength();

    for (auto valueIdx = 0; valueIdx < fieldSize; ++valueIdx, ++fieldIt) {
        rgbData[valueIdx] = diffraction::wavelengthToRgb(wavelength, fieldIt->real());
    }

    diffraction::Transformable{rgbData}.transform(diffraction::RgbNormTransform{});
}

void fillTextureWithRgb(std::vector<diffraction::RGBData>& rgbData,
                        std::vector<std::uint8_t>& texturePixels) {
    constexpr auto kMaxChannelValue = 255;

    for (auto pixelIdx = 0; pixelIdx < kWindowWidth * kWindowHeight; ++pixelIdx) {
        auto pixelComponentIdx = pixelIdx * kPixelComponentsCount;

        texturePixels[pixelComponentIdx] = rgbData[pixelIdx].r * kMaxChannelValue;
        texturePixels[pixelComponentIdx + 1] = rgbData[pixelIdx].g * kMaxChannelValue;
        texturePixels[pixelComponentIdx + 2] = rgbData[pixelIdx].b * kMaxChannelValue;
    }
}

bool handleEvents(sf::RenderWindow& window, diffraction::MonochromaticField& field) {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            return false;
            ;
        } else if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyEvent->scancode) {
                case sf::Keyboard::Scancode::Up:
                    field.setWavelength(
                        std::min(kMaximumWavelength, field.getWavelength() + kWavelengthStep));
                    break;
                case sf::Keyboard::Scancode::Down:
                    field.setWavelength(
                        std::max(kMinimumWavelength, field.getWavelength() - kWavelengthStep));
                    break;
                case sf::Keyboard::Scancode::Escape:
                    return false;
                    break;
                default:
                    break;
            }
        }
    }

    return true;
}
}  // namespace
