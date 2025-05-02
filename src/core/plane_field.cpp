#include <algorithm>

#include "core/plane_field.hpp"
#include "core/attributes.hpp"

namespace diffraction {
    PlaneField::PlaneField(std::size_t xSize, std::size_t ySize, const FieldValue *rawData) : 
        xSize_(xSize), ySize_(ySize), fieldValues_(xSize * ySize) {

        if (rawData == nullptr) {
            DIFFRACTION_CRITICAL("rawData pointer is nullptr");
        }

        if (xSize == 0 || ySize == 0) {
            return;
        }
    
        std::copy(rawData, rawData + fieldValues_.size(), fieldValues_.begin());
    }
}
