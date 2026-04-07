#pragma once

#include "filters/Filter.h"

namespace imgproc {

class ArtisticLensFilter : public Filter {
public:
    ArtisticLensFilter(int pixel_block_size, double vignette_radius, double contrast_factor);

    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    int pixel_block_size_;
    double vignette_radius_;
    double contrast_factor_;
};

}  // namespace imgproc
