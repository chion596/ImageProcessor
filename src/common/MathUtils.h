#pragma once

#include <algorithm>
#include <cstdint>
#include <cmath>

#include "common/Constants.h"

namespace imgproc {

inline double Clamp01(double value) {
    return std::clamp(value, KMinColor, KMaxColor);
}

inline std::uint8_t DoubleToByte(double value) {
    const double clamped = Clamp01(value);
    return static_cast<std::uint8_t>(std::round(clamped * KByteScale));
}

inline double ByteToDouble(std::uint8_t value) {
    return static_cast<double>(value) / KByteScale;
}

inline std::int32_t ClampIndex(std::int32_t value, std::int32_t low, std::int32_t high) {
    return std::clamp(value, low, high);
}

}  // namespace imgproc
