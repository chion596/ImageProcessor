#pragma once

#include <cstdint>

namespace imgproc {

constexpr std::uint16_t KBmpSignature = 0x4D42;
constexpr std::uint32_t KBitmapInfoHeaderSize = 40;
constexpr std::uint16_t KBmpPlanes = 1;
constexpr std::uint16_t KBmpBitCount = 24;
constexpr std::uint32_t KBmpCompressionRgb = 0;

constexpr double KMinColor = 0.0;
constexpr double KMaxColor = 1.0;
constexpr double KByteScale = 255.0;
constexpr double KPi = 3.14159265358979323846;

constexpr double KLuminanceRed = 0.299;
constexpr double KLuminanceGreen = 0.587;
constexpr double KLuminanceBlue = 0.114;

constexpr double KKernelZero = 0.0;
constexpr double KKernelSide = -1.0;
constexpr double KSharpenCenter = 5.0;
constexpr double KEdgeCenter = 4.0;

constexpr double KPixelationRadiusMultiplier = 1.1;
constexpr double KMidColor = 0.5;
constexpr double KMaxVignetteFactor = 1.0;
constexpr double KArtisticLensFalloffMultiplier = 1.5;

constexpr double KGaussianRadiusMultiplier = 3.0;
constexpr double KGaussianDenominatorMultiplier = 2.0;

}  // namespace imgproc
