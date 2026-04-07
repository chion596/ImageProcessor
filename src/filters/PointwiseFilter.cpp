#include "filters/PointwiseFilter.h"

namespace imgproc {

void PointwiseFilter::Apply(Image& image) const {
    Image result(image.Width(), image.Height());

    for (std::int32_t y = 0; y < image.Height(); ++y) {
        for (std::int32_t x = 0; x < image.Width(); ++x) {
            result.At(x, y) = Transform(image.At(x, y), x, y, image).Clamped();
        }
    }

    image = std::move(result);
}

}  // namespace imgproc
