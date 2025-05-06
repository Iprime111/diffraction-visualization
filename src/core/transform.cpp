#include "core/transform.hpp"

#include "core/attributes.hpp"
#include "core/plane_field.hpp"

namespace diffraction {
void MultiplyTransformer::transform(PlaneField& input) const {
    auto& multiplier = multiplierRef_.get();

    if (multiplier.getXSize() != input.getXSize()) {
        DIFFRACTION_CRITICAL("X size of multiplied plane fields is not equal");
    }

    if (multiplier.getYSize() != input.getYSize()) {
        DIFFRACTION_CRITICAL("X size of multiplied plane fields is not equal");
    }

    auto multiplierIt = multiplier.begin();
    for (auto inputIt = input.begin(), inputEnd = input.end(); inputIt != inputEnd;
         ++inputIt, ++multiplierIt) {
        *inputIt *= *multiplierIt;
    }
}

void NormTransformer::transform(PlaneField& input) const {
    for (auto& value : input) {
        value = std::norm(value);
    }
}

void FillTransformer::transform(PlaneField& input) const {
    std::fill(input.begin(), input.end(), value_);
}
}  // namespace diffraction
