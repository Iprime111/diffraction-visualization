#pragma once

#include <functional>
#include <stdexcept>

#include "core/attributes.hpp"
#include "core/types.hpp"
#include "core/plane_field.hpp"

namespace diffraction {
namespace transformers {
// TODO fftw 2d fft and ifft wrapper classes
// https://www.fftw.org/fftw2_doc/fftw_2.html

class FFT2D final : NonCopyable {
  public:
    explicit FFT2D(PlaneField& fft) : fftRef_(fft) {
        FieldValue* data = fft.getRawData();
        
        plan_ = fftw_plan_dft_2d(fft.getYSize(), fft.getXSize(),
                                 &fieldValueToFftw(*data),
                                 &fieldValueToFftw(*data),
                                 FFTW_FORWARD,
                                 FFTW_ESTIMATE); // без измерения производительности
    
        if (!plan_) {
            throw std::runtime_error("Failed to create FFTW plan");
        }
    } 

    ~FFT2D() {
        fftw_destroy_plan(plan_);
    }

    // TODO use in-place transform FFTW_IN_PLACE https://www.fftw.org/fftw2_doc/fftw_3.html#SEC19
    PlaneField& transform(PlaneField& input) const;

  private:
    fftw_plan plan_;
    std::reference_wrapper<PlaneField> fftRef_;
};

class FFT2DInverse final : NonCopyable {
  public:
    // TODO same as FFT2D
    explicit FFT2DInverse(PlaneField& fftInverse) : fftInverseRef_(fftInverse) {
        const auto& rows = fftInverse.getYSize();
        const auto& cols = fftInverse.getXSize(); // TODO: Возможно кринж и лучше поля создать

        normFactor_ = 1.0 / (rows * cols);

        FieldValue *data = fftInverse.getRawData();
        
        plan_ = fftw_plan_dft_2d(rows, cols,
                                &fieldValueToFftw(*data),
                                &fieldValueToFftw(*data),
                                FFTW_BACKWARD,
                                FFTW_ESTIMATE);
        
        if (!plan_) {
            throw std::runtime_error("Failed to create FFTW plan");
        }
    }

    ~FFT2DInverse() {
        fftw_destroy_plan(plan_);
    }

    PlaneField& transform(PlaneField& input) const;

  private:
    double normFactor_;
    fftw_plan plan_;
    std::reference_wrapper<PlaneField> fftInverseRef_;
};
} // namespace transformers
} // namespace diffraction
