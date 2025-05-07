#pragma once

#include <functional>

#include "core/fft.hpp"
#include "core/plane_field.hpp"

namespace diffraction {
struct LightPropagationSettings {
    double propagationLength;
    double screenXNm;
    double screenYNm;
};

class LightPropagationTransform {
  public:
    LightPropagationTransform(LightPropagationSettings settings, const FFTPlan<FFT2D>& fftPlan,
                              const FFTPlan<FFT2DInverse>& fftInversePlan)
        : settings_(settings), fftPlan_(fftPlan), fftInversePlan_(fftInversePlan) {}

    void transform(MonochromaticField& input) const;

  private:
    LightPropagationSettings settings_;

    std::reference_wrapper<const FFTPlan<FFT2D>> fftPlan_;
    std::reference_wrapper<const FFTPlan<FFT2DInverse>> fftInversePlan_;
};
}  // namespace diffraction
