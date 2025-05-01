#pragma once

#include "core/plane_field.hpp"

namespace diffraction {
namespace transformers {
// TODO fftw 2d fft and ifft wrapper classes
// https://www.fftw.org/fftw2_doc/fftw_2.html

class FFT2D final {
  public:
    // TODO use in-place transform FFTW_IN_PLACE https://www.fftw.org/fftw2_doc/fftw_3.html#SEC19
    PlaneField& transform(PlaneField& input);

  private:
    fftw_plan plan_;
    // TODO ...
};

class FFT2DInverse final {
  public:
    // TODO same as FFT2D
    PlaneField& transform(PlaneField& input);

  private:
    // TODO ...
};
}
} // namespace diffraction
