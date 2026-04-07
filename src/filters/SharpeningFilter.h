#pragma once

#include "filters/MatrixFilter.h"

namespace imgproc {

class SharpeningFilter : public MatrixFilter {
public:
    std::string Name() const override;

protected:
    Kernel3x3 Kernel() const override;
};

}  // namespace imgproc
