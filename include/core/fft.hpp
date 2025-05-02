#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>

#include "core/attributes.hpp"
#include "core/types.hpp"
#include "core/plane_field.hpp"

namespace diffraction {

class FFT2D final : NonCopyable {
  public:
    FFT2D() = default;

    void transform(PlaneField& input) const;
};

class FFT2DInverse final : NonCopyable {
  public:
    FFT2DInverse() = default;

    void transform(PlaneField& input) const;
};
} // namespace diffraction
