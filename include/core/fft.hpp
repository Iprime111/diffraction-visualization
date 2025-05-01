#pragma once

#include <functional>
#include <stdexcept>

#include "core/attributes.hpp"
#include "core/types.hpp"
#include "core/plane_field.hpp"

namespace diffraction {

class FFT2D final : NonCopyable {
  public:
    explicit FFT2D(PlaneField& fft);

    ~FFT2D();

    PlaneField& transform(PlaneField& input) const;

  private:
    fftw_plan plan_;
    std::reference_wrapper<PlaneField> fftRef_;
};

class FFT2DInverse final : NonCopyable {
  public:
    explicit FFT2DInverse(PlaneField& fftInverse);

    ~FFT2DInverse();

    PlaneField& transform(PlaneField& input) const;

  private:
    fftw_plan plan_;
    std::reference_wrapper<PlaneField> fftInverseRef_;
};
} // namespace diffraction
