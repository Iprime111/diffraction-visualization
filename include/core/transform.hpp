#include <functional>

#include "core/plane_field.hpp"

namespace diffraction {
template<typename Transformer, typename DataType>
DataType &transform(Transformer& transformer, DataType& input) {
    return transformer.transform(input);
}

class MultiplyTransformer final {
  public:
    MultiplyTransformer(PlaneField& multiplier) : multiplierRef_(multiplier) {}

    PlaneField& transform(PlaneField& input);

  private:
    std::reference_wrapper<PlaneField> multiplierRef_;
};
} // namespace diffraction
