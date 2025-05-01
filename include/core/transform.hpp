#pragma once

#include <functional>

#include "core/attributes.hpp"
#include "core/plane_field.hpp"

namespace diffraction {
template<typename Transformer, typename DataType>
void transform(const Transformer& transformer, DataType& input) {
    transformer.transform(input);
}

template<typename InnerType>
class Transformable final : NonCopyable {
  public:
    explicit Transformable(InnerType& input) : transformedObject_(input) {}

    template<typename Transformer>
    Transformable<InnerType>& transform(const Transformer& transformer) {
        diffraction::transform(transformer, transformedObject_.get());
        return *this;
    }

  private:
    std::reference_wrapper<InnerType> transformedObject_;
};

class MultiplyTransformer final : NonCopyable {
  public:
    explicit MultiplyTransformer(PlaneField& multiplier) : multiplierRef_(multiplier) {}

    PlaneField& transform(PlaneField& input) const;

  private:
    std::reference_wrapper<PlaneField> multiplierRef_;
};

class NormTransformer final {
  public:
    PlaneField& transform(PlaneField& input) const;
};
} // namespace diffraction
