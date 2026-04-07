#pragma once

#include <string>

#include "core/Image.h"

namespace imgproc {

class Filter {
public:
    virtual ~Filter() = default;

    virtual void Apply(Image& image) const = 0;
    virtual std::string Name() const = 0;
};

}  // namespace imgproc
