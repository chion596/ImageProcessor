#pragma once

#include <array>

#include "filters/Filter.h"

namespace imgproc {

using Kernel3x3 = std::array<std::array<double, 3>, 3>;

class MatrixFilter : public Filter {
public:
    void Apply(Image& image) const override;

protected:
    virtual Kernel3x3 Kernel() const = 0;

    virtual Image PrepareInput(const Image& image) const;

    virtual Color FinalizeColor(const Color& value, std::int32_t x, std::int32_t y, const Image& prepared_input) const;

private:
    static Image ApplyKernel3x3(const Image& source, const Kernel3x3& kernel);
};

}  // namespace imgproc
