#include "app/HelpPrinter.h"

#include <ostream>

void HelpPrinter::Print(std::ostream& output) {
    output << "Image processor for 24-bit uncompressed BMP files\n\n"
           << "Usage:\n"
           << "  image_processor <input.bmp> <output.bmp> [filters...]\n\n"
           << "Filters:\n"
           << "  -crop width height              Crop top-left area\n"
           << "  -gs                             Convert to grayscale\n"
           << "  -neg                            Convert to negative\n"
           << "  -sharp                          Apply sharpening kernel\n"
           << "  -edge threshold                 Edge detection after grayscale\n"
           << "  -blur sigma                     Gaussian blur (sigma > 0)\n"
           << "  -lens pixel_size radius factor  Artistic filter\n\n"
           << "Example:\n"
           << "  image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5\n";
}
