#pragma once

#include <cstdint>

#include "common/Constants.h"

namespace imgproc {

#pragma pack(push, 1)

struct BitmapFileHeader {
    std::uint16_t bf_type = KBmpSignature;
    std::uint32_t bf_size = 0;
    std::uint16_t bf_reserved_1 = 0;
    std::uint16_t bf_reserved_2 = 0;
    std::uint32_t bf_off_bits = 0;
};

struct BitmapInfoHeader {
    std::uint32_t bi_size = KBitmapInfoHeaderSize;
    std::int32_t bi_width = 0;
    std::int32_t bi_height = 0;
    std::uint16_t bi_planes = KBmpPlanes;
    std::uint16_t bi_bit_count = KBmpBitCount;
    std::uint32_t bi_compression = KBmpCompressionRgb;
    std::uint32_t bi_size_image = 0;
    std::int32_t bi_x_pels_per_meter = 0;
    std::int32_t bi_y_pels_per_meter = 0;
    std::uint32_t bi_clr_used = 0;
    std::uint32_t bi_clr_important = 0;
};

#pragma pack(pop)

}  // namespace imgproc
