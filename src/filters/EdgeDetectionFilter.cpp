#include "filters/EdgeDetectionFilter.h"

#include "common/Constants.h"
#include "common/Exceptions.h"
#include "filters/GrayscaleFilter.h"

namespace imgproc {

EdgeDetectionFilter::EdgeDetectionFilter(double threshold) : threshold_(threshold) {
    if (threshold < KMinColor || threshold > KMaxColor) {
        throw ArgumentError("Edge threshold must be in [0, 1]");
    }
}

Kernel3x3 EdgeDetectionFilter::Kernel() const {
    return {{{KKernelZero, KKernelSide, KKernelZero},
             {KKernelSide, KEdgeCenter, KKernelSide},
             {KKernelZero, KKernelSide, KKernelZero}}};
}

Image EdgeDetectionFilter::PrepareInput(const Image& image) const {
    Image prepared = image;
    GrayscaleFilter grayscale;
    grayscale.Apply(prepared);
    return prepared;
}

Color EdgeDetectionFilter::FinalizeColor(const Color& value, std::int32_t, std::int32_t, const Image&) const {
    const double intensity = value.r;
    const double bw = (intensity > threshold_) ? KMaxColor : KMinColor;
    return {bw, bw, bw};
}

std::string EdgeDetectionFilter::Name() const {
    return "edge";
}

}  // namespace imgproc
