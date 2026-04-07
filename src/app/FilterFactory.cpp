#include "app/FilterFactory.h"

#include <string>
#include <vector>

#include "common/Exceptions.h"
#include "filters/ArtisticLensFilter.h"
#include "filters/CropFilter.h"
#include "filters/EdgeDetectionFilter.h"
#include "filters/GaussianBlurFilter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/NegativeFilter.h"
#include "filters/SharpeningFilter.h"

namespace {

double ParseDouble(const std::string& value, const std::string& filter_name) {
    try {
        std::size_t position = 0;
        const double result = std::stod(value, &position);
        if (position != value.size()) {
            throw std::invalid_argument("trailing characters");
        }
        return result;
    } catch (...) {
        throw ArgumentError("Invalid numeric parameter for filter '" + filter_name + "': " + value);
    }
}

int ParseInt(const std::string& value, const std::string& filter_name) {
    try {
        std::size_t position = 0;
        const int result = std::stoi(value, &position);
        if (position != value.size()) {
            throw std::invalid_argument("trailing characters");
        }
        return result;
    } catch (...) {
        throw ArgumentError("Invalid integer parameter for filter '" + filter_name + "': " + value);
    }
}

void ExpectCount(const std::vector<std::string>& params, std::size_t expected, const std::string& filter_name) {
    if (params.size() != expected) {
        throw ArgumentError("Filter '" + filter_name + "' expects " + std::to_string(expected) + " parameter(s), got " +
                            std::to_string(params.size()));
    }
}

}  // namespace

std::unique_ptr<imgproc::Filter> FilterFactory::Create(const std::string& name,
                                                       const std::vector<std::string>& raw_params) {

    if (name == "gs") {
        ExpectCount(raw_params, 0, name);
        return std::make_unique<imgproc::GrayscaleFilter>();
    }

    if (name == "neg") {
        ExpectCount(raw_params, 0, name);
        return std::make_unique<imgproc::NegativeFilter>();
    }

    if (name == "sharp") {
        ExpectCount(raw_params, 0, name);
        return std::make_unique<imgproc::SharpeningFilter>();
    }

    if (name == "crop") {
        ExpectCount(raw_params, 2, name);
        const int width = ParseInt(raw_params[0], name);
        const int height = ParseInt(raw_params[1], name);
        return std::make_unique<imgproc::CropFilter>(width, height);
    }

    if (name == "edge") {
        ExpectCount(raw_params, 1, name);
        const double threshold = ParseDouble(raw_params[0], name);
        return std::make_unique<imgproc::EdgeDetectionFilter>(threshold);
    }

    if (name == "blur") {
        ExpectCount(raw_params, 1, name);
        const double sigma = ParseDouble(raw_params[0], name);
        return std::make_unique<imgproc::GaussianBlurFilter>(sigma);
    }

    if (name == "lens") {
        ExpectCount(raw_params, 3, name);
        const int pixel_size = ParseInt(raw_params[0], name);
        const double vignette_radius = ParseDouble(raw_params[1], name);
        const double contrast_factor = ParseDouble(raw_params[2], name);
        return std::make_unique<imgproc::ArtisticLensFilter>(pixel_size, vignette_radius, contrast_factor);
    }

    throw ArgumentError("Unknown filter: " + name);
}
