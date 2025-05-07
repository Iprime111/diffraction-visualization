#include <fftw3.h>
#include <fmt/format.h>

#include "core/attributes.hpp"
#include "core/fft.hpp"

namespace diffraction {
template<>
FFTPlan<FFT2D>::FFTPlan(std::size_t xSize, std::size_t ySize) : xSize_(xSize), ySize_(ySize) {
    // Allocate array only for plan creation due to fftw limitations
    fftw_complex *planArray = fftw_alloc_complex(xSize_ * ySize_);

    if (!planArray) {
        DIFFRACTION_CRITICAL("Unable to allocate temporary buffer");
    }

    plan_ = fftw_plan_dft_2d(ySize_, xSize_, planArray, planArray, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_free(planArray);

    if (!plan_) {
        DIFFRACTION_CRITICAL("Failed to create FFTW 2D inverse plan");
    }
}

template<>
FFTPlan<FFT2DInverse>::FFTPlan(std::size_t xSize, std::size_t ySize) : xSize_(xSize), ySize_(ySize) {
    // Allocate array only for plan creation due to fftw limitations
    fftw_complex *planArray = fftw_alloc_complex(xSize_ * ySize_);

    if (!planArray) {
        DIFFRACTION_CRITICAL("Unable to allocate temporary buffer");
    }

    plan_ = fftw_plan_dft_2d(ySize_, xSize_, planArray, planArray, FFTW_BACKWARD, FFTW_ESTIMATE);

    fftw_free(planArray);

    if (!plan_) {
        DIFFRACTION_CRITICAL("Failed to create FFTW 2D inverse plan");
    }
}

void FFT2D::transform(PlaneField& input) const {
    auto *dataPtr = &fieldValueToFftw(*input.getRawData());

    fftw_execute_dft(plan_.get().getPlan(), dataPtr, dataPtr);
}

void FFT2DInverse::transform(PlaneField& input) const {
    auto *dataPtr = &fieldValueToFftw(*input.getRawData());

    fftw_execute_dft(plan_.get().getPlan(), dataPtr, dataPtr);

    double normCoeff = input.getXSize() * input.getYSize();

    for (auto &value : input) {
        value /= normCoeff;
    }
}
}  // namespace diffraction
