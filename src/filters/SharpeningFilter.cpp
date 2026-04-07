#include "filters/SharpeningFilter.h"

#include "common/Constants.h"

namespace imgproc {

Kernel3x3 SharpeningFilter::Kernel() const {
    return {{{KKernelZero, KKernelSide, KKernelZero},
             {KKernelSide, KSharpenCenter, KKernelSide},
             {KKernelZero, KKernelSide, KKernelZero}}};
}

std::string SharpeningFilter::Name() const {
    return "sharp";
}

}  // namespace imgproc
