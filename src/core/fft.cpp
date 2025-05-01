#include <fftw3.h>
#include <stdexcept>
#include <fmt/format.h>

#include "core/fft.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"
#include "core/types.hpp"

namespace diffraction {
FFT2D::FFT2D(PlaneField& fft) : fftRef_(fft) {
        
    fftw_complex* data = &fieldValueToFftw(*fft.getRawData());
    
    plan_ = fftw_plan_dft_2d(fft.getYSize(), fft.getXSize(),
                             data,
                             data,
                             FFTW_FORWARD,
                             FFTW_ESTIMATE);

    if (!plan_) {
        throw std::runtime_error("Failed to create FFTW plan");
    }
} 

FFT2D::~FFT2D() {
    if (plan_) {
        fftw_destroy_plan(plan_);
    }
}

PlaneField& FFT2D::transform(PlaneField& input) const {
    auto& fft = fftRef_.get();

    if (fft.getXSize() != input.getXSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    if (fft.getYSize() != input.getYSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    auto *dataPtr = &fieldValueToFftw(*input.getRawData());
    fftw_execute_dft(plan_, dataPtr, dataPtr);

    return input;
}

FFT2DInverse::FFT2DInverse(PlaneField& fftInverse) : 
    fftInverseRef_(fftInverse)
{
    const auto rows = fftInverse.getYSize();
    const auto cols = fftInverse.getXSize();

    fftw_complex* data = &fieldValueToFftw(*fftInverse.getRawData());
    
    plan_ = fftw_plan_dft_2d(rows, cols,
                             data,
                             data,
                             FFTW_BACKWARD,
                             FFTW_ESTIMATE);
    
    if (!plan_) {
        throw std::runtime_error("Failed to create FFTW plan");
    }
}

FFT2DInverse::~FFT2DInverse() {
    if (plan_) {
        fftw_destroy_plan(plan_);
    }
}

PlaneField& FFT2DInverse::transform(PlaneField& input) const {
    auto& fftInverse = fftInverseRef_.get();

    if (fftInverse.getXSize() != input.getXSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    if (fftInverse.getYSize() != input.getYSize()) {
        throw std::runtime_error{"X size of multiplied plane fields is not equal"};
    }

    auto *dataPtr = &fieldValueToFftw(*input.getRawData());
    fftw_execute_dft(plan_, dataPtr, dataPtr);

    return input;
}
} // namespace diffraction