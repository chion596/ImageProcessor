#pragma once

#include <memory>
#include <string>
#include <vector>

#include "filters/Filter.h"

class FilterFactory {
public:
    static std::unique_ptr<imgproc::Filter> Create(const std::string& name, const std::vector<std::string>& raw_params);
};
