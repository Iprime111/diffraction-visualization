#include <functional>

#include "core/attributes.hpp"
#include "core/plane_field.hpp"

namespace diffraction {
template<typename Transformer, typename DataType>
DataType &transform(Transformer& transformer, DataType& input) {
    return transformer.transform(input);
}

template<typename InnerType>
class Transformable final : NonCopyable {
  public:
    Transformable(InnerType& input) : transformedObject_(input) {}

    template<typename Transformer>
    Transformable<InnerType>& transform(Transformer& transformer) {
        transform(transformer, transformedObject_.get());
        return *this;
    }

  private:
    std::reference_wrapper<InnerType> transformedObject_;
};

class MultiplyTransformer final : NonCopyable {
  public:
    MultiplyTransformer(PlaneField& multiplier) : multiplierRef_(multiplier) {}

    PlaneField& transform(PlaneField& input);

  private:
    std::reference_wrapper<PlaneField> multiplierRef_;
};
} // namespace diffraction
