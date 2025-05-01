#pragma once

#include <complex>
#include <fftw3.h>

namespace diffraction {
using FieldValue = std::complex<double>;

inline fftw_complex& fieldValueToFftw(FieldValue& value) {
    return reinterpret_cast<double(&)[2]>(value);
}

inline FieldValue& fftwToFieldValue(fftw_complex& value) {
    return reinterpret_cast<FieldValue&>(value);
}
} // namespace diffraction
