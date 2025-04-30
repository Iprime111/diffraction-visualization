#pragma once

#include <cstddef>
#include <cstring>
#include <vector>

#include "core/types.hpp"
#include "core/attributes.hpp"

namespace diffraction {

class MonochromaticPlaneField final {
  private:
    std::vector<FieldValue> fieldValues_;

    using ContainerType = decltype(fieldValues_);
  public:
    using iterator = ContainerType::iterator;
    using const_iterator = ContainerType::const_iterator;
    using reference = ContainerType::reference;
    using const_reference = ContainerType::const_reference;
    using difference_type = ContainerType::difference_type;
    using value_type = ContainerType::value_type;
    using size_type = ContainerType::size_type;

    // TODO move methods to cpp file? (This will disable inlining optimizations)

    class RowProxy final {
        friend MonochromaticPlaneField;

      public:
        RowProxy(const RowProxy&) = delete;
        RowProxy &operator=(const RowProxy&) = delete;

        DIFFRACTION_NODISCARD FieldValue &operator[](std::size_t column) {
            return *(rowIterator_ + column);
        }

      private:
        RowProxy(const iterator &rowIterator, std::size_t rowSize) :
            rowIterator_(rowIterator), rowSize_(rowSize) {}

        iterator rowIterator_;
        std::size_t rowSize_{0};
    };

    MonochromaticPlaneField(std::size_t xSize, std::size_t ySize) : 
        xSize_(xSize), ySize_(ySize), fieldValues_(xSize * ySize) {}

    MonochromaticPlaneField(std::size_t xSize, std::size_t ySize, const FieldValue *rawData);

    MonochromaticPlaneField(const MonochromaticPlaneField&) = default;
    MonochromaticPlaneField &operator=(const MonochromaticPlaneField&) = default;

    MonochromaticPlaneField(MonochromaticPlaneField&&) = default;
    MonochromaticPlaneField &operator=(MonochromaticPlaneField&&) = default;

    DIFFRACTION_NODISCARD RowProxy operator[](std::size_t row) {
        return RowProxy{fieldValues_.begin() + row * xSize_, xSize_};
    }

    DIFFRACTION_NODISCARD FieldValue *getRawData() {
        return fieldValues_.data();  
    }

  private:
    std::size_t xSize_{0};
    std::size_t ySize_{0};
};
} // namespace diffraction
