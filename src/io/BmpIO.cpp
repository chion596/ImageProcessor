#include "io/BmpIO.h"

#include <cstdlib>
#include <fstream>
#include <vector>

#include "common/Constants.h"
#include "common/Exceptions.h"
#include "common/MathUtils.h"
#include "io/BmpHeaders.h"

namespace imgproc {
namespace {

std::uint32_t ComputeRowSize(std::int32_t width) {
    return static_cast<std::uint32_t>(((width * 3) + 3) / 4 * 4);
}

template <typename T>
void ReadExact(std::ifstream& input, T& value, const std::string& what) {
    input.read(reinterpret_cast<char*>(&value), sizeof(T));
    if (!input) {
        throw FormatError("Failed to read " + what);
    }
}

}  // namespace

Image LoadBmp24(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw IoError("Cannot open input file: " + path);
    }

    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;
    ReadExact(input, file_header, "BMP file header");
    ReadExact(input, info_header, "BMP info header");

    if (file_header.bf_type != KBmpSignature) {
        throw FormatError("Invalid BMP signature: only BMP files are supported");
    }
    if (info_header.bi_size != KBitmapInfoHeaderSize) {
        throw FormatError("Only BITMAPINFOHEADER-based BMP files are supported");
    }
    if (info_header.bi_planes != KBmpPlanes) {
        throw FormatError("Invalid BMP: biPlanes must be 1");
    }
    if (info_header.bi_bit_count != KBmpBitCount) {
        throw FormatError("Only 24-bit BMP files are supported");
    }
    if (info_header.bi_compression != KBmpCompressionRgb) {
        throw FormatError("Compressed BMP files are not supported");
    }
    if (info_header.bi_width <= 0 || info_header.bi_height == 0) {
        throw FormatError("BMP image has invalid dimensions");
    }

    const std::int32_t width = info_header.bi_width;
    const std::int32_t height = std::abs(info_header.bi_height);
    const bool bottom_up = info_header.bi_height > 0;

    Image image(width, height);

    const std::uint32_t row_size = ComputeRowSize(width);
    input.seekg(static_cast<std::streamoff>(file_header.bf_off_bits), std::ios::beg);
    if (!input) {
        throw FormatError("Invalid pixel data offset in BMP");
    }

    std::vector<std::uint8_t> row(row_size, 0);
    for (std::int32_t file_row = 0; file_row < height; ++file_row) {
        input.read(reinterpret_cast<char*>(row.data()), static_cast<std::streamsize>(row_size));
        if (!input) {
            throw FormatError("Unexpected end of file while reading BMP pixel data");
        }

        const std::int32_t image_y = bottom_up ? (height - 1 - file_row) : file_row;
        for (std::int32_t x = 0; x < width; ++x) {
            const std::size_t offset = static_cast<std::size_t>(x) * 3u;
            const std::uint8_t b = row[offset];
            const std::uint8_t g = row[offset + 1];
            const std::uint8_t r = row[offset + 2];
            image.At(x, image_y) = Color::FromRgbBytes(r, g, b);
        }
    }

    return image;
}

void SaveBmp24(const std::string& path, const Image& image) {
    if (image.Empty()) {
        throw ArgumentError("Cannot save an empty image");
    }

    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw IoError("Cannot open output file: " + path);
    }

    const std::int32_t width = image.Width();
    const std::int32_t height = image.Height();
    const std::uint32_t row_size = ComputeRowSize(width);
    const std::uint32_t image_size = row_size * static_cast<std::uint32_t>(height);

    BitmapFileHeader file_header;
    file_header.bf_off_bits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    file_header.bf_size = file_header.bf_off_bits + image_size;

    BitmapInfoHeader info_header;
    info_header.bi_width = width;
    info_header.bi_height = height;
    info_header.bi_size_image = image_size;

    output.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    output.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));
    if (!output) {
        throw IoError("Failed to write BMP headers to: " + path);
    }

    std::vector<std::uint8_t> row(row_size, 0);
    for (std::int32_t y = height - 1; y >= 0; --y) {
        std::fill(row.begin(), row.end(), 0);

        for (std::int32_t x = 0; x < width; ++x) {
            const Color color = image.At(x, y).Clamped();
            const std::size_t offset = static_cast<std::size_t>(x) * 3u;

            row[offset] = DoubleToByte(color.b);
            row[offset + 1] = DoubleToByte(color.g);
            row[offset + 2] = DoubleToByte(color.r);
        }

        output.write(reinterpret_cast<const char*>(row.data()), static_cast<std::streamsize>(row_size));
        if (!output) {
            throw IoError("Failed to write BMP pixel data to: " + path);
        }
    }
}

}  // namespace imgproc
