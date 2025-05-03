#pragma once

#include <compare>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>

#include "core/types.hpp"
#include "core/attributes.hpp"

namespace diffraction {
namespace detail {
struct PlainFieldStorage {
    explicit PlainFieldStorage(std::size_t storageSize);
    ~PlainFieldStorage();

    PlainFieldStorage(const PlainFieldStorage& other);
    PlainFieldStorage& operator=(const PlainFieldStorage& other);

    PlainFieldStorage(PlainFieldStorage&& other);
    PlainFieldStorage& operator=(PlainFieldStorage&& other);

    FieldValue* data_;
    FieldValue* dataFinish_;
};
} // namespace detail

class PlaneField : detail::PlainFieldStorage {
  public:
    template<typename DataType>
    class Iterator final : public std::random_access_iterator_tag {
      public:
        using value_type = DataType;
        using difference_type = std::ptrdiff_t;
        using reference = std::add_lvalue_reference_t<DataType>;
        using pointer = std::add_pointer_t<DataType>;
        using iterator_category = std::random_access_iterator_tag;

        friend PlaneField;

        Iterator() : itPointer_(nullptr) {}

        Iterator<DataType>& operator+=(difference_type diff) {
            itPointer_ += diff;
            return *this;
        }

        Iterator<DataType>& operator-=(difference_type diff) {
            itPointer_ -= diff;
            return *this;
        }

        Iterator<DataType> operator+(difference_type diff) const {
            return Iterator<DataType>{itPointer_ + diff};
        }

        Iterator<DataType> operator-(difference_type diff) const {
            return Iterator<DataType>{itPointer_ - diff};
        }

        difference_type operator-(const Iterator<DataType>& other) const {
            return itPointer_ - other.itPointer_;
        }

        reference operator[](difference_type diff) {
            return *(itPointer_ + diff);
        }

        std::strong_ordering operator<=>(const Iterator<DataType>& other) const {
            if (itPointer_ > other.itPointer_) {
                return std::strong_ordering::greater;
            } else if (itPointer_ < other.itPointer_) {
                return std::strong_ordering::less;
            }

            return std::strong_ordering::equal;
        }

        bool operator==(const Iterator<DataType>& other) const {
            return itPointer_ == other.itPointer_;
        }

        bool operator!=(const Iterator<DataType>& other) const = default;

        Iterator<DataType>& operator--() {
            itPointer_--;
            return *this;
        }

        Iterator<DataType> operator--(int) {
            return Iterator<DataType>{itPointer_--};
        }

        Iterator<DataType>& operator++() {
            itPointer_++;
            return *this;
        }

        Iterator<DataType> operator++(int) {
            return Iterator<DataType>{itPointer_++};
        }

        reference operator*() {
            return *itPointer_;
        }

        pointer operator->() {
            return itPointer_;
        }

      private:
        Iterator(pointer ptr) : itPointer_(ptr) {}

        pointer itPointer_;  
    };

    using iterator = Iterator<FieldValue>;
    using const_iterator = Iterator<const FieldValue>;
    using reference = iterator::reference;
    using const_reference = const_iterator::reference;
    using pointer = iterator::pointer;
    using const_pointer = const_iterator::pointer;
    using difference_type = std::ptrdiff_t;
    using value_type = FieldValue;
    using size_type = std::size_t;

    // TODO move methods to cpp file? (This will disable inlining optimizations)

    class RowProxy final : NonCopyable {
        friend PlaneField;

      public:
        DIFFRACTION_NODISCARD FieldValue &operator[](std::size_t column) {
            return *(rowIterator_ + column);
        }

      private:
        RowProxy(const iterator &rowIterator) :
            rowIterator_(rowIterator) {}

        iterator rowIterator_;
    };

    PlaneField(std::size_t xSize, std::size_t ySize) : 
        PlainFieldStorage(xSize * ySize), xSize_(xSize), ySize_(ySize) {}

    PlaneField(std::size_t xSize, std::size_t ySize, const FieldValue *rawData);

    PlaneField(const PlaneField&) = default;
    PlaneField &operator=(const PlaneField&) = default;

    PlaneField(PlaneField&&) = default;
    PlaneField &operator=(PlaneField&&) = default;

    virtual ~PlaneField() = default;

    DIFFRACTION_NODISCARD FieldValue *getRawData() {
        return data_;
    }

    DIFFRACTION_NODISCARD auto getXSize() const {
        return xSize_;
    }

    DIFFRACTION_NODISCARD auto getYSize() const {
        return ySize_;
    }

    DIFFRACTION_NODISCARD auto begin() {
        return iterator{data_};
    }

    DIFFRACTION_NODISCARD auto end() {
        return iterator{dataFinish_};
    }

    DIFFRACTION_NODISCARD auto begin() const {
        return const_iterator{data_};
    }

    DIFFRACTION_NODISCARD auto end() const {
        return const_iterator{dataFinish_};
    }

    DIFFRACTION_NODISCARD RowProxy operator[](std::size_t row) {
        return RowProxy{begin() + row * xSize_};
    }

  private:
    std::size_t xSize_{0};
    std::size_t ySize_{0};
};

class MonochromaticField final : public PlaneField {
  public:
    MonochromaticField(std::size_t xSize, std::size_t ySize, double wavelength) : 
        PlaneField(xSize, ySize), wavelength_(wavelength) {}

    MonochromaticField(std::size_t xSize, std::size_t ySize, FieldValue *rawData, double wavelength) : 
        PlaneField(xSize, ySize, rawData), wavelength_(wavelength) {}

    DIFFRACTION_NODISCARD auto getWavelength() const {
        return wavelength_;
    }
  private:
    double wavelength_;
};
} // namespace diffraction
