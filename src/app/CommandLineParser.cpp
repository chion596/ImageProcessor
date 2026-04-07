#include "app/CommandLineParser.h"

#include <string>
#include <utility>
#include <vector>

#include "app/FilterFactory.h"
#include "common/Exceptions.h"

Config CommandLineParser::Parse(int argc, char** argv) {
    if (argc < 3) {
        throw ArgumentError("Usage: image_processor <input.bmp> <output.bmp> [-filter params ...]");
    }

    Config config;
    config.input_path = argv[1];
    config.output_path = argv[2];

    int index = 3;
    while (index < argc) {
        const std::string token = argv[index];
        if (token.empty() || token[0] != '-') {
            throw ArgumentError("Expected filter name starting with '-' , got: " + token);
        }

        const std::string filter_name = token.substr(1);
        ++index;

        std::vector<std::string> raw_params;
        while (index < argc && argv[index][0] != '-') {
            raw_params.emplace_back(argv[index]);
            ++index;
        }

        config.filters.push_back(FilterFactory::Create(filter_name, raw_params));
    }

    return config;
}
