#include <stdexcept>

#include "core/plane.hpp"

namespace diffraction {
    MonochromaticPlaneField::MonochromaticPlaneField(std::size_t xSize, std::size_t ySize, const FieldValue *rawData) : 
        xSize_(xSize), ySize_(ySize), fieldValues_(xSize * ySize) {

        if (rawData == nullptr) {
            throw std::runtime_error{"rawData pointer is nullptr"};
        }

        if (xSize == 0 || ySize == 0) {
            return;
        }

        std::memcpy(fieldValues_.data(), rawData, fieldValues_.size() * sizeof(FieldValue));
    }
}
