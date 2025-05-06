#include <algorithm>
#include <fftw3.h>
#include <utility>

#include "core/plane_field.hpp"
#include "core/attributes.hpp"
#include "core/types.hpp"

namespace diffraction {
namespace detail {
PlainFieldStorage::PlainFieldStorage(std::size_t storageSize) {
    // Allocate aligned storage
    data_ = reinterpret_cast<FieldValue *>(fftw_alloc_complex(storageSize));

    if (!data_) {
        DIFFRACTION_CRITICAL("PlaneField allocation error");
    }

    dataFinish_ = data_ + storageSize;
}

PlainFieldStorage::~PlainFieldStorage() {
    fftw_free(data_);
}

PlainFieldStorage::PlainFieldStorage(const PlainFieldStorage& other) : PlainFieldStorage(other.dataFinish_ - other.data_) {
    std::copy(other.data_, other.dataFinish_, data_);
}

PlainFieldStorage& PlainFieldStorage::operator=(const PlainFieldStorage& other) {
    auto otherSize = other.dataFinish_ - other.data_;

    if (dataFinish_ - data_ != otherSize) {
        // Reallocate storage
        fftw_free(data_);
        data_ = reinterpret_cast<FieldValue *>(fftw_alloc_complex(otherSize));

        if (!data_) {
            DIFFRACTION_CRITICAL("PlaneField reallocation error");
        }

        dataFinish_ = other.dataFinish_;
    }

    std::copy(other.data_, other.dataFinish_, data_);
    
    return *this;
}

PlainFieldStorage::PlainFieldStorage(PlainFieldStorage&& other) : data_(nullptr), dataFinish_(nullptr) {
    std::swap(dataFinish_, other.dataFinish_);
    std::swap(data_, other.data_);
}

PlainFieldStorage& PlainFieldStorage::operator=(PlainFieldStorage&& other) {
    std::swap(dataFinish_, other.dataFinish_);
    std::swap(data_, other.data_);

    return *this;
}
} // namespace detail

PlaneField::PlaneField(std::size_t xSize, std::size_t ySize, const FieldValue *rawData) : 
    PlainFieldStorage(xSize * ySize), xSize_(xSize), ySize_(ySize) {

    if (rawData == nullptr) {
        DIFFRACTION_CRITICAL("rawData pointer is nullptr");
    }

    if (xSize == 0 || ySize == 0) {
        return;
    }

    auto dataSize = dataFinish_ - data_;
    std::copy(rawData, rawData + dataSize, data_);
}
} // namespace diffraction
