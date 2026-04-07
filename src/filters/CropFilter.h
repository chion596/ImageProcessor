#pragma once

#include "filters/Filter.h"

namespace imgproc {

class CropFilter : public Filter {
public:
    CropFilter(std::int32_t width, std::int32_t height);

    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    std::int32_t width_;
    std::int32_t height_;
};

}  // namespace imgproc
