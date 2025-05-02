#pragma once

#include <vector>

#include "core/attributes.hpp"

namespace diffraction {
struct XYZData final {
    double x{0};
    double y{0};
    double z{0};
};

struct RGBData final {
    double r{0};
    double g{0};
    double b{0};
};

const RGBData wavelengthToRgb(double wavelength, double intensity);
const XYZData wavelengthToXyz(double wavelength, double intensity);

class RgbNormTransform final : NonCopyable {
  public:
    explicit RgbNormTransform(double cutoff = 1.0) : cutoff_(1.0) {
        if (cutoff_ <= 0) {
            DIFFRACTION_CRITICAL("RGB cutoff can't be less than zero");
        }
    }

    void transform(std::vector<RGBData>& data) const;

  private:
    double cutoff_{1.0};
};
}
