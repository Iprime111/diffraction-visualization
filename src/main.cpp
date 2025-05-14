#include <fmt/base.h>
#include <CLI/Validators.hpp>
#include <exception>

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
#include <filesystem>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <vector>

#include "SFML/Graphics/Text.hpp"
#include "conversions/wavelength_lookup.hpp"
#include "core/aperture.hpp"
#include "core/attributes.hpp"
#include "core/fft.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"
#include "physics/propagation.hpp"

constexpr auto kWindowWidth = 1024;
constexpr auto kWindowHeight = 1024;
constexpr auto kPixelComponentsCount = 4;  // RGBA
constexpr auto kWavelengthStep = 5;

// TODO remove
constexpr auto kDefaultWavelength = 630.0;
constexpr auto kMinimumWavelength = 380.0;
constexpr auto kMaximumWavelength = 779.0;

constexpr auto kWavelengthTextSize = 24.f;
constexpr auto kWavelengthTextColor = sf::Color::White;

constexpr auto kLengthTextSize = 24.f;
constexpr auto kLengthTextColor = sf::Color::White;

constexpr auto kDefaultPropagationLength = 8e8;
constexpr auto kLengthStep = 5e7;

constexpr auto kAppTitle = "Diffraction";
constexpr auto kJetBreainsFont = "/fonts/JetBrainsMonoNLNerdFont-Regular.ttf";

namespace {
void getIntensityRgbData(diffraction::MonochromaticField& field, std::vector<diffraction::RGBData>& rgbData);
void fillTextureWithRgb(std::vector<diffraction::RGBData>& rgbData, std::vector<std::uint8_t>& texturePixels);
bool handleEvents(sf::RenderWindow& window, diffraction::MonochromaticField& field, sf::Text& wavelengthText,
                  diffraction::LightPropagationSettings& settings, sf::Text& lengthText);
void updateWavelengthText(sf::Text& wavelengthText, double currentWavelength);
void updateLengthText(sf::Text& lengthText, double length);
}  // namespace

int main(int argc, char** argv) {
    // TODO handle exceptions

    CLI::App app{kAppTitle};

    std::string aperturePath;
    app.add_option("-f,--aperture_file,aperture_file", aperturePath, "Path to input aperture image file")
        ->required()
        ->check(CLI::ExistingFile);

    double wavelength = kDefaultWavelength;
    app.add_option("-w,--wavelength", wavelength, "Start wavelength")
        ->check(CLI::Range{kMinimumWavelength, kMaximumWavelength});

    double propagationLength = kDefaultPropagationLength;
    app.add_option("-l,--length", propagationLength, "Propagation length (nm)");

    CLI11_PARSE(app, argc, argv);

    try {
        sf::Image apertureImage;
        if (!apertureImage.loadFromFile(aperturePath)) {
            DIFFRACTION_CRITICAL("Failed to load aperture image: {}", aperturePath);
        }

        diffraction::Aperture aperture{std::move(apertureImage), kWindowWidth, kWindowHeight};

        sf::RenderWindow window{sf::VideoMode({kWindowWidth, kWindowHeight}), kAppTitle, sf::Style::Close,
                                sf::State::Windowed};

        sf::Texture texture{sf::Vector2u{kWindowWidth, kWindowHeight}};

        std::vector<std::uint8_t> texturePixels(kWindowWidth * kWindowHeight * kPixelComponentsCount);
        std::vector<diffraction::RGBData> rgbData(kWindowWidth * kWindowHeight);

        // Paint default texture white to enable alpha channel
        std::fill(texturePixels.begin(), texturePixels.end(), 255);
        texture.update(texturePixels.data());

        sf::Sprite textureSprite{texture};

        diffraction::MonochromaticField startField{kWindowWidth, kWindowHeight, wavelength};
        diffraction::Transformable{startField}
            .transform(diffraction::FillTransformer{1.0})
            .transform(diffraction::MultiplyTransformer{aperture});

        diffraction::LightPropagationSettings lightSettings{
            .propagationLength = propagationLength,
            .screenXNm = 6e6,
            .screenYNm = 6e6,
        };

        diffraction::FFTPlan<diffraction::FFT2D> fftPlan{kWindowWidth, kWindowHeight};
        diffraction::FFTPlan<diffraction::FFT2DInverse> ifftPlan{kWindowWidth, kWindowHeight};

        std::filesystem::path executablePath(argv[0]);
        std::string executeDir = executablePath.parent_path().string();
        std::string fontPath = executeDir + kJetBreainsFont;

        sf::Font font(fontPath);
        sf::Text wavelengthText(font);
        wavelengthText.setCharacterSize(kWavelengthTextSize);
        wavelengthText.setFillColor(kWavelengthTextColor);
        wavelengthText.setPosition(sf::Vector2f(10.f, window.getSize().y - kWavelengthTextSize - 10.f));

        sf::Text lengthText(font);
        wavelengthText.setCharacterSize(kLengthTextSize);
        wavelengthText.setFillColor(kLengthTextColor);
        wavelengthText.setPosition(sf::Vector2f(10.f, window.getSize().y - kLengthTextSize - 6.f));

        updateWavelengthText(wavelengthText, startField.getWavelength());
        updateLengthText(lengthText, propagationLength);

        while (window.isOpen()) {
            if (!handleEvents(window, startField, wavelengthText, lightSettings, lengthText)) {
                window.close();
                break;
            }

            auto resultField{startField};

            diffraction::Transformable{resultField}.transform(
                diffraction::LightPropagationTransform{lightSettings, fftPlan, ifftPlan});

            getIntensityRgbData(resultField, rgbData);
            fillTextureWithRgb(rgbData, texturePixels);
            texture.update(texturePixels.data());

            window.draw(textureSprite);
            window.draw(wavelengthText);
            window.draw(lengthText);
            window.display();
        }
    } catch (const std::exception& ex) {
        fmt::println("ERROR: {}", ex.what());
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

void updateWavelengthText(sf::Text& wavelengthText, double currentWavelength) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << "Wavelength: " << currentWavelength << " nm";
    wavelengthText.setString(ss.str());
}

void updateLengthText(sf::Text& lengthText, double length) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << "Propagation length: " << length / 1e7 << " cm";
    lengthText.setString(ss.str());
}

bool handleEvents(sf::RenderWindow& window, diffraction::MonochromaticField& field, sf::Text& wavelengthText,
                  diffraction::LightPropagationSettings& settings, sf::Text& lengthText) {
    bool wavelengthChanged = false;
    bool lengthChanged = false;

    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            return false;
        } else if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyEvent->scancode) {
                case sf::Keyboard::Scancode::Equal:
                    field.setWavelength(
                        std::min(kMaximumWavelength, field.getWavelength() + kWavelengthStep));

                    wavelengthChanged = true;
                    break;
                case sf::Keyboard::Scancode::Hyphen:
                    field.setWavelength(
                        std::max(kMinimumWavelength, field.getWavelength() - kWavelengthStep));

                    wavelengthChanged = true;
                    break;
                case sf::Keyboard::Scancode::Left:
                    settings.propagationLength -= kLengthStep;
                    lengthChanged = true;
                    break;
                case sf::Keyboard::Scancode::Right:
                    settings.propagationLength += kLengthStep;
                    lengthChanged = true;
                    break;
                case sf::Keyboard::Scancode::Escape:
                    return false;
                default:
                    break;
            }
        }
    }

    if (wavelengthChanged) {
        updateWavelengthText(wavelengthText, field.getWavelength());
    }

    if (lengthChanged) {
        updateLengthText(lengthText, settings.propagationLength);
    }

    return true;
}
}  // namespace
