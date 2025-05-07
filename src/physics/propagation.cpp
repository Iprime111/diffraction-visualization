#include <cmath>
#include <complex>
#include <cstddef>

#include "core/fft.hpp"
#include "core/transform.hpp"
#include "core/types.hpp"
#include "physics/propagation.hpp"

namespace {
constexpr auto kPi = 3.1415;
constexpr auto kIm = std::complex<double>{0.0, 1.0};
}  // namespace

namespace diffraction {
void LightPropagationTransform::transform(MonochromaticField& input) const {
    auto wavelength = input.getWavelength();
    auto xSize = input.getXSize();
    auto ySize = input.getYSize();

    Transformable{input}
        .transform(FFT2D{fftPlan_.get()})
        .transform(CoordinatesTransformer{
            [wavelength, this, xSize, ySize](std::size_t x, std::size_t y, FieldValue& value) {
                long long shiftX = (x + xSize / 2) % xSize - xSize / 2;
                long long shiftY = (y + ySize / 2) % ySize - ySize / 2;

                auto wavenumberZ = std::sqrt(std::pow(2 * kPi / wavelength, 2) -
                                             std::pow(shiftX * kPi / settings_.screenXNm, 2) -
                                             std::pow(shiftY * kPi / settings_.screenYNm, 2));
                value *= std::exp(kIm * settings_.propagationLength * wavenumberZ);
            }})
        .transform(FFT2DInverse{fftInversePlan_.get()});
}
}  // namespace diffraction
