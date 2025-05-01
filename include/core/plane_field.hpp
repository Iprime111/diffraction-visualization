#pragma once

#include <cstddef>
#include <cstring>
#include <vector>

#include "core/types.hpp"
#include "core/attributes.hpp"

namespace diffraction {

class PlaneField final {
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
        friend PlaneField;

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

    PlaneField(std::size_t xSize, std::size_t ySize) : 
        xSize_(xSize), ySize_(ySize), fieldValues_(xSize * ySize) {}

    PlaneField(std::size_t xSize, std::size_t ySize, const FieldValue *rawData);

    PlaneField(const PlaneField&) = default;
    PlaneField &operator=(const PlaneField&) = default;

    PlaneField(PlaneField&&) = default;
    PlaneField &operator=(PlaneField&&) = default;

    DIFFRACTION_NODISCARD RowProxy operator[](std::size_t row) {
        return RowProxy{fieldValues_.begin() + row * xSize_, xSize_};
    }

    DIFFRACTION_NODISCARD FieldValue *getRawData() {
        return fieldValues_.data();  
    }

    DIFFRACTION_NODISCARD auto getXSize() const {
        return xSize_;
    }

    DIFFRACTION_NODISCARD auto getYSize() const {
        return ySize_;
    }

    DIFFRACTION_NODISCARD auto begin() {
        return fieldValues_.begin();
    }

    DIFFRACTION_NODISCARD auto end() {
        return fieldValues_.begin();
    }

    DIFFRACTION_NODISCARD auto begin() const {
        return fieldValues_.begin();
    }

    DIFFRACTION_NODISCARD auto end() const {
        return fieldValues_.end();
    }

  private:
    std::size_t xSize_{0};
    std::size_t ySize_{0};
};
} // namespace diffraction
