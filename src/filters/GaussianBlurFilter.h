#pragma once

#include <vector>

#include "filters/Filter.h"

namespace imgproc {

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(double sigma);

    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    std::vector<double> BuildKernel() const;

private:
    double sigma_;
};

}  // namespace imgproc
