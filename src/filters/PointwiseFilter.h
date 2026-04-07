#pragma once

#include "filters/Filter.h"

namespace imgproc {

class PointwiseFilter : public Filter {
public:
    void Apply(Image& image) const override;

protected:
    virtual Color Transform(const Color& color, std::int32_t x, std::int32_t y, const Image& source) const = 0;
};

}  // namespace imgproc
