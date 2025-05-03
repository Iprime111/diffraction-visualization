#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>

#include "core/aperture.hpp"

namespace diffraction {
Aperture::Aperture(const sf::Image& image, size_t xSize, size_t ySize, double threshold) :
    PlaneField(xSize, ySize) {
    
    constexpr double kEpsilon = 1e-10;
    if (kEpsilon < (0.0 - threshold) || (threshold - 1.0) > kEpsilon) {
        DIFFRACTION_CRITICAL("Threshold must be between 0.0 and 1.0");
    }

    sf::Image scaledImage = scaleImage(image, xSize, ySize);
    convertToPlaneField(scaledImage, threshold);
}

sf::Image Aperture::scaleImage(const sf::Image& image, std::size_t xSize, std::size_t ySize) const {
    if (image.getSize().x == xSize && image.getSize().y == ySize) {
        return image;
    }

    sf::Texture texture;
    if (!texture.loadFromImage(image)) {
        DIFFRACTION_CRITICAL("Failed to create texture from image");
    }

    sf::Sprite sprite(texture);
    float scaleX = static_cast<float>(xSize) / image.getSize().x;
    float scaleY = static_cast<float>(ySize) / image.getSize().y;
    sprite.setScale(sf::Vector2f(scaleX, scaleY));

    sf::RenderTexture renderTexture(sf::Vector2u(xSize, ySize));

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(sprite);
    renderTexture.display();

    return renderTexture.getTexture().copyToImage();
} 

void Aperture::convertToPlaneField(const sf::Image& image, double threshold) {
    const std::size_t& ySize = getYSize();
    const std::size_t& xSize = getXSize();

    const double maxIntensity = 255.0 * 3.0;

    for (std::size_t y = 0; y < ySize; ++y) {
        for (std::size_t x = 0; x < xSize; ++x) {
            sf::Color pixel = image.getPixel(sf::Vector2u(x, y));
            double intensity = (pixel.r + pixel.g + pixel.b) / maxIntensity;

            double result = (intensity > threshold) ? 1.0 : 0.0;
            (*this)[y][x] = {result, 0.0};
        }
    }
}
} // namespace diffraction