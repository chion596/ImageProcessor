#pragma once

#include "filters/MatrixFilter.h"

namespace imgproc {

class EdgeDetectionFilter : public MatrixFilter {
public:
    explicit EdgeDetectionFilter(double threshold);

    std::string Name() const override;

protected:
    Kernel3x3 Kernel() const override;
    Image PrepareInput(const Image& image) const override;
    Color FinalizeColor(const Color& value, std::int32_t x, std::int32_t y, const Image& prepared_input) const override;

private:
    double threshold_;
};

}  // namespace imgproc
