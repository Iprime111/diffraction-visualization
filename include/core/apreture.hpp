#pragma once

#include <SFML/Graphics.hpp>
#include <stdexcept>

#include "core/plane_field.hpp"
#include "core/attributes.hpp"

namespace diffraction {
class Aperture : PlaneField {
  public:
    Aperture(const std::string& filename, size_t xSize, size_t ySize) :
        PlaneField(xSize, ySize)
    {
        sf::Image sourceImage = loadImage(filename);
        
        sf::Image scaledImage = scaleImage(sourceImage, xSize, ySize);
        
        convertToPlaneField(scaledImage);
    }
    
  private:
    sf::Image loadImage(const std::string& filename) const {
        sf::Image image;
        if (!image.loadFromFile(filename)) {
            throw std::runtime_error("Failed to load aperture image: " + filename);
        }
        return image;
    }

    sf::Image scaleImage(const sf::Image& source, std::size_t xSize, std::size_t ySize) const 
    {
        sf::Texture texture;
        if (!texture.loadFromImage(source)) {
            throw std::runtime_error("Failed to create texture from image");
        }

        sf::Sprite sprite(texture);
        sprite.setScale({
            static_cast<float>(xSize) / source.getSize().x,
            static_cast<float>(ySize) / source.getSize().y
        });

        // // sf::RenderTexture render_texture;
        // // if (!render_texture.create(xSize, ySize)) {
        // //     throw std::runtime_error("Failed to create render texture");
        // // }

        // render_texture.clear(sf::Color::Black);
        // render_texture.draw(sprite);
        // render_texture.display();

        // return render_texture.getTexture().copyToImage();
    }

    void convertToPlaneField(const sf::Image& image) {
        for (std::size_t yCoord = 0; yCoord < getYSize(); ++yCoord) {
            for (std::size_t xCoord = 0; xCoord < getXSize(); ++xCoord) {
                sf::Color pixel = image.getPixel(sf::Vector2u(xCoord, yCoord));
                double intensity = (pixel.r + pixel.g + pixel.b) / (3.0 * 255.0);
                (*this)[yCoord][xCoord] = {intensity, 0.0};
            }
        }
    }
};

// TODO aperture loader (from file)

// TODO aperture loader (from function)
} // namespace diffraction
