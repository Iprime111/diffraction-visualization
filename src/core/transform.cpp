#include <stdexcept>

#include "core/transform.hpp"
#include "core/plane_field.hpp"

namespace diffraction {
PlaneField& MultiplyTransformer::transform(PlaneField& input) {
    auto& multiplier = multiplierRef_.get();

    if (multiplier.getXSize() != input.getXSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    if (multiplier.getYSize() != input.getYSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    for (auto inputIt = input.begin(), multiplierIt = multiplier.begin(), inputEnd = input.end();
         inputIt != inputEnd; ++inputIt, ++multiplierIt) {

        *inputIt *= *multiplierIt;
    }

    return input;
}
} // namespace diffraction
