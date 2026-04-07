#pragma once

#include <string>

#include "core/Image.h"

namespace imgproc {

Image LoadBmp24(const std::string& path);
void SaveBmp24(const std::string& path, const Image& image);

}  // namespace imgproc
