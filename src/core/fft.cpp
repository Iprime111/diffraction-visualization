#include <fftw3.h>
#include <stdexcept>

#include "core/fft.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"

namespace diffraction {
PlaneField& transformers::FFT2D::transform(PlaneField& input) const {
    auto& fft = fftRef_.get();

    if (fft.getXSize() != input.getXSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    if (fft.getYSize() != input.getYSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    FieldValue *data = input.getRawData();
    fftw_execute_dft(plan_, &fieldValueToFftw(*data), &fieldValueToFftw(*data));

    return input;
}

PlaneField& transformers::FFT2DInverse::transform(PlaneField& input) const {
    auto& fftInverse = fftInverseRef_.get();

    if (fftInverse.getXSize() != input.getXSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    if (fftInverse.getYSize() != input.getYSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    FieldValue *data = input.getRawData();
    fftw_execute_dft(plan_, &fieldValueToFftw(*data), &fieldValueToFftw(*data));
    
    for (auto inputIt = input.begin(), inputEnd = input.end(); 
         inputIt != inputEnd; ++inputIt) {

        fftwToFieldValue(fieldValueToFftw(*inputIt)) *= normFactor_;
    }
        
    return input;
}

} // namespace diffraction