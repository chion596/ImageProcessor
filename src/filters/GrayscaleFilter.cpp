#include "filters/GrayscaleFilter.h"

namespace imgproc {

Color GrayscaleFilter::Transform(const Color& color, std::int32_t, std::int32_t, const Image&) const {
    const double gray = Color::Luminance(color);
    return {gray, gray, gray};
}

std::string GrayscaleFilter::Name() const {
    return "gs";
}

}  // namespace imgproc
