#pragma once

#include <cstdint>

#include "common/Constants.h"
#include "common/MathUtils.h"

namespace imgproc {

struct Color {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    Color() = default;
    Color(double red, double green, double blue) : r(red), g(green), b(blue) {
    }

    void Clamp() {
        r = Clamp01(r);
        g = Clamp01(g);
        b = Clamp01(b);
    }

    [[nodiscard]] Color Clamped() const {
        Color copy = *this;
        copy.Clamp();
        return copy;
    }

    [[nodiscard]] static Color FromRgbBytes(std::uint8_t red, std::uint8_t green, std::uint8_t blue) {
        return {ByteToDouble(red), ByteToDouble(green), ByteToDouble(blue)};
    }

    [[nodiscard]] static double Luminance(const Color& color) {
        return KLuminanceRed * color.r + KLuminanceGreen * color.g + KLuminanceBlue * color.b;
    }
};

}  // namespace imgproc
