#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include "core/plane_field.hpp"

namespace diffraction {
class Aperture : public PlaneField {
  public:
    Aperture(const sf::Image& image, size_t xSize, size_t ySize, double threshold=0.5);

  private:
    sf::Image scaleImage(const sf::Image& image, std::size_t xSize, std::size_t ySize) const;

    void convertToPlaneField(const sf::Image& image, double threshold);
};

// TODO aperture loader (from function)
}  // namespace diffraction
