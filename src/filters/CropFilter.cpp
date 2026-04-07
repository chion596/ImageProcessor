#include "filters/CropFilter.h"

#include <algorithm>

#include "common/Exceptions.h"

namespace imgproc {

CropFilter::CropFilter(std::int32_t width, std::int32_t height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw ArgumentError("Crop width and height must be positive");
    }
}

void CropFilter::Apply(Image& image) const {

    const std::int32_t new_width = std::min(width_, image.Width());
    const std::int32_t new_height = std::min(height_, image.Height());

    Image cropped(new_width, new_height);
    for (std::int32_t y = 0; y < new_height; ++y) {
        for (std::int32_t x = 0; x < new_width; ++x) {
            cropped.At(x, y) = image.At(x, y);
        }
    }

    image = std::move(cropped);
}

std::string CropFilter::Name() const {
    return "crop";
}

}  // namespace imgproc
