#include <fftw3.h>
#include <stdexcept>
#include <fmt/format.h>

#include "core/fft.hpp"
#include "core/plane_field.hpp"
#include "core/transform.hpp"
#include "core/types.hpp"

// TODO: use fftw_malloc in PlaneField instead of vector

namespace diffraction {
void FFT2D::transform(PlaneField& input) const {
    auto *dataPtr = &fieldValueToFftw(*input.getRawData());

    fftw_plan plan_ = fftw_plan_dft_2d(input.getYSize(), input.getXSize(),
                                       dataPtr,
                                       dataPtr,
                                       FFTW_FORWARD,
                                       FFTW_ESTIMATE | FFTW_UNALIGNED);

    if (!plan_) {
        throw std::runtime_error("Failed to create FFTW plan");
    }

    fftw_execute_dft(plan_, dataPtr, dataPtr);

    fftw_destroy_plan(plan_);
}

void FFT2DInverse::transform(PlaneField& input) const {
    auto *dataPtr = &fieldValueToFftw(*input.getRawData());

    fftw_plan plan_ = fftw_plan_dft_2d(input.getYSize(), input.getXSize(),
                                       dataPtr,
                                       dataPtr,
                                       FFTW_BACKWARD,
                                       FFTW_ESTIMATE | FFTW_UNALIGNED);

    if (!plan_) {
        throw std::runtime_error("Failed to create FFTW plan");
    }

    fftw_execute_dft(plan_, dataPtr, dataPtr);

    fftw_destroy_plan(plan_);
}
} // namespace diffraction