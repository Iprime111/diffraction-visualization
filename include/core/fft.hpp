#pragma once

#include <fftw3.h>
#include <cstddef>
#include <functional>

#include "core/attributes.hpp"
#include "core/plane_field.hpp"

namespace diffraction {

template<typename Transform>
class FFTPlan final : NonCopyable {
  public:
    FFTPlan(std::size_t xSize, std::size_t ySize) {
        static_assert("FFTPlan accepts only FFT2D and FFT2DInverse as a template parameter");
    }

    ~FFTPlan() {
        fftw_destroy_plan(plan_);
    }

    DIFFRACTION_NODISCARD const fftw_plan& getPlan() const {
        return plan_;
    }

    DIFFRACTION_NODISCARD std::size_t getXSize() const {
        return xSize_;
    }

    DIFFRACTION_NODISCARD std::size_t getYSize() const {
        return ySize_;
    }

  private:
    fftw_plan plan_;
    std::size_t xSize_{0};
    std::size_t ySize_{0};
};

class FFT2D;
class FFT2DInverse;

template<>
FFTPlan<FFT2D>::FFTPlan(std::size_t xSize, std::size_t ySize);

template<>
FFTPlan<FFT2DInverse>::FFTPlan(std::size_t xSize, std::size_t ySize);

class FFT2D final : NonCopyable {
  public:
    FFT2D(const FFTPlan<FFT2D>& plan) : plan_(plan) {}

    void transform(PlaneField& input) const;

  private:
    std::reference_wrapper<const FFTPlan<FFT2D>> plan_;
};

class FFT2DInverse final : NonCopyable {
  public:
    FFT2DInverse(const FFTPlan<FFT2DInverse>& plan) : plan_(plan) {}
    void transform(PlaneField& input) const;

  private:
    std::reference_wrapper<const FFTPlan<FFT2DInverse>> plan_;
};
}  // namespace diffraction
