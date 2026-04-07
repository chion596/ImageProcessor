#pragma once

#include <memory>
#include <string>
#include <vector>

#include "filters/Filter.h"

struct Config {
    std::string input_path;
    std::string output_path;
    std::vector<std::unique_ptr<imgproc::Filter>> filters;
};
