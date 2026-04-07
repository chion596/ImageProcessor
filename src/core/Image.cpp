#include "core/Image.h"

#include <algorithm>

#include "common/MathUtils.h"

namespace imgproc {

Image::Image(std::int32_t width, std::int32_t height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw ArgumentError("Image size must be positive");
    }
    pixels_.resize(static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_));
}

std::int32_t Image::Width() const noexcept {
    return width_;
}

std::int32_t Image::Height() const noexcept {
    return height_;
}

bool Image::Empty() const noexcept {
    return pixels_.empty();
}

Color& Image::At(std::int32_t x, std::int32_t y) {
    return pixels_.at(Index(x, y));
}

const Color& Image::At(std::int32_t x, std::int32_t y) const {
    return pixels_.at(Index(x, y));
}

const Color& Image::AtClamped(std::int32_t x, std::int32_t y) const {
    const std::int32_t nx = ClampIndex(x, 0, width_ - 1);
    const std::int32_t ny = ClampIndex(y, 0, height_ - 1);
    return pixels_[Index(nx, ny)];
}

void Image::Fill(const Color& color) {
    std::fill(pixels_.begin(), pixels_.end(), color);
}

std::size_t Image::Index(std::int32_t x, std::int32_t y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw ArgumentError("Image pixel index is out of range");
    }
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) + static_cast<std::size_t>(x);
}

}  // namespace imgproc
