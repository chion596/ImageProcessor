#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "common/Exceptions.h"
#include "core/Color.h"

namespace imgproc {

class Image {
public:
    Image() = default;
    Image(std::int32_t width, std::int32_t height);

    [[nodiscard]] std::int32_t Width() const noexcept;
    [[nodiscard]] std::int32_t Height() const noexcept;
    [[nodiscard]] bool Empty() const noexcept;

    Color& At(std::int32_t x, std::int32_t y);
    const Color& At(std::int32_t x, std::int32_t y) const;

    const Color& AtClamped(std::int32_t x, std::int32_t y) const;

    void Fill(const Color& color);

private:
    [[nodiscard]] std::size_t Index(std::int32_t x, std::int32_t y) const;

private:
    std::int32_t width_ = 0;
    std::int32_t height_ = 0;
    std::vector<Color> pixels_;
};

}  // namespace imgproc
