#include "filters/ArtisticLensFilter.h"

#include <algorithm>
#include <cmath>

#include "common/Constants.h"
#include "common/Exceptions.h"

namespace imgproc {

ArtisticLensFilter::ArtisticLensFilter(int pixel_block_size, double vignette_radius, double contrast_factor)
    : pixel_block_size_(pixel_block_size), vignette_radius_(vignette_radius), contrast_factor_(contrast_factor) {
    if (pixel_block_size <= 0) {
        throw ArgumentError("Lens filter pixel_block_size must be positive");
    }
    if (vignette_radius < KMinColor) {
        throw ArgumentError("Lens filter vignette_radius must be non-negative");
    }
    if (contrast_factor <= KMinColor) {
        throw ArgumentError("Lens filter contrast_factor must be positive");
    }
}

void ArtisticLensFilter::Apply(Image& image) const {
    const std::int32_t width = image.Width();
    const std::int32_t height = image.Height();
    const double center_x = static_cast<double>(width) / KGaussianDenominatorMultiplier;
    const double center_y = static_cast<double>(height) / KGaussianDenominatorMultiplier;
    const double max_falloff = std::max(center_x, center_y) * KArtisticLensFalloffMultiplier;

    Image original = image;

    for (std::int32_t y = 0; y < height; ++y) {
        for (std::int32_t x = 0; x < width; ++x) {
            const double dx = static_cast<double>(x) - center_x;
            const double dy = static_cast<double>(y) - center_y;
            const double dist = std::sqrt(dx * dx + dy * dy);

            Color color = original.At(x, y);

            if (dist > vignette_radius_ * KPixelationRadiusMultiplier) {
                const std::int32_t block_x = (x / pixel_block_size_) * pixel_block_size_;
                const std::int32_t block_y = (y / pixel_block_size_) * pixel_block_size_;
                color = original.AtClamped(block_x, block_y);
            }

            if (dist < vignette_radius_) {
                color.r = KMidColor + contrast_factor_ * (color.r - KMidColor);
                color.g = KMidColor + contrast_factor_ * (color.g - KMidColor);
                color.b = KMidColor + contrast_factor_ * (color.b - KMidColor);
            }

            if (dist > vignette_radius_) {
                const double factor = std::max(KMinColor, KMaxVignetteFactor - (dist - vignette_radius_) / max_falloff);
                color.r *= factor;
                color.g *= factor;
                color.b *= factor;
            }

            image.At(x, y) = color.Clamped();
        }
    }
}

std::string ArtisticLensFilter::Name() const {
    return "lens";
}

}  // namespace imgproc
