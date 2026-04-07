#pragma once

#include "filters/PointwiseFilter.h"

namespace imgproc {

class GrayscaleFilter : public PointwiseFilter {
public:
    std::string Name() const override;

protected:
    Color Transform(const Color& color, std::int32_t x, std::int32_t y, const Image& source) const override;
};

}  // namespace imgproc
