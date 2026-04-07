#include "filters/NegativeFilter.h"

namespace imgproc {

Color NegativeFilter::Transform(const Color& color, std::int32_t, std::int32_t, const Image&) const {
    return {1.0 - color.r, 1.0 - color.g, 1.0 - color.b};
}

std::string NegativeFilter::Name() const {
    return "neg";
}

}  // namespace imgproc
