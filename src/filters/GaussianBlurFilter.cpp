#include "filters/GaussianBlurFilter.h"

#include <cmath>
#include <vector>

#include "common/Constants.h"
#include "common/Exceptions.h"

namespace imgproc {

GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma) {
    if (sigma <= 0.0) {
        throw ArgumentError("Gaussian blur sigma must be positive");
    }
}

std::vector<double> GaussianBlurFilter::BuildKernel() const {

    const int radius = static_cast<int>(std::ceil(KGaussianRadiusMultiplier * sigma_));
    std::vector<double> kernel(static_cast<std::size_t>(2 * radius + 1), 0.0);

    double sum = 0.0;
    const double denom = KGaussianDenominatorMultiplier * sigma_ * sigma_;
    const double norm = KMaxColor / std::sqrt(KGaussianDenominatorMultiplier * KPi * sigma_ * sigma_);

    for (int offset = -radius; offset <= radius; ++offset) {
        const double value = norm * std::exp(-(offset * offset) / denom);
        kernel[static_cast<std::size_t>(offset + radius)] = value;
        sum += value;
    }

    for (double& value : kernel) {
        value /= sum;
    }

    return kernel;
}

void GaussianBlurFilter::Apply(Image& image) const {
    const std::vector<double> kernel = BuildKernel();
    const int radius = static_cast<int>((kernel.size() - 1) / 2);

    Image horizontal(image.Width(), image.Height());

    for (std::int32_t y = 0; y < image.Height(); ++y) {
        for (std::int32_t x = 0; x < image.Width(); ++x) {
            Color acc{0.0, 0.0, 0.0};

            for (int offset = -radius; offset <= radius; ++offset) {
                const Color& sample = image.AtClamped(x + offset, y);
                const double weight = kernel[static_cast<std::size_t>(offset + radius)];
                acc.r += sample.r * weight;
                acc.g += sample.g * weight;
                acc.b += sample.b * weight;
            }

            horizontal.At(x, y) = acc;
        }
    }

    Image vertical(image.Width(), image.Height());
    for (std::int32_t y = 0; y < image.Height(); ++y) {
        for (std::int32_t x = 0; x < image.Width(); ++x) {
            Color acc{0.0, 0.0, 0.0};

            for (int offset = -radius; offset <= radius; ++offset) {
                const Color& sample = horizontal.AtClamped(x, y + offset);
                const double weight = kernel[static_cast<std::size_t>(offset + radius)];
                acc.r += sample.r * weight;
                acc.g += sample.g * weight;
                acc.b += sample.b * weight;
            }

            vertical.At(x, y) = acc.Clamped();
        }
    }

    image = std::move(vertical);
}

std::string GaussianBlurFilter::Name() const {
    return "blur";
}

}  // namespace imgproc
