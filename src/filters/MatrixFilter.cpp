#include "filters/MatrixFilter.h"

namespace imgproc {

Image MatrixFilter::ApplyKernel3x3(const Image& source, const Kernel3x3& kernel) {
    Image result(source.Width(), source.Height());

    for (std::int32_t y = 0; y < source.Height(); ++y) {
        for (std::int32_t x = 0; x < source.Width(); ++x) {
            Color sum{0.0, 0.0, 0.0};

            for (std::int32_t dy = -1; dy <= 1; ++dy) {
                for (std::int32_t dx = -1; dx <= 1; ++dx) {
                    const double factor = kernel[dy + 1][dx + 1];
                    const Color& neighbor = source.AtClamped(x + dx, y + dy);
                    sum.r += neighbor.r * factor;
                    sum.g += neighbor.g * factor;
                    sum.b += neighbor.b * factor;
                }
            }

            result.At(x, y) = sum;
        }
    }

    return result;
}

void MatrixFilter::Apply(Image& image) const {
    const Image prepared = PrepareInput(image);
    Image result = ApplyKernel3x3(prepared, Kernel());

    for (std::int32_t y = 0; y < result.Height(); ++y) {
        for (std::int32_t x = 0; x < result.Width(); ++x) {
            result.At(x, y) = FinalizeColor(result.At(x, y), x, y, prepared).Clamped();
        }
    }

    image = std::move(result);
}

Image MatrixFilter::PrepareInput(const Image& image) const {
    return image;
}

Color MatrixFilter::FinalizeColor(const Color& value, std::int32_t, std::int32_t, const Image&) const {
    return value.Clamped();
}

}  // namespace imgproc
