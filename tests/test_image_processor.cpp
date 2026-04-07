#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "app/CommandLineParser.h"
#include "app/FilterFactory.h"
#include "common/Constants.h"
#include "common/Exceptions.h"
#include "core/Image.h"
#include "filters/CropFilter.h"
#include "filters/EdgeDetectionFilter.h"
#include "filters/GaussianBlurFilter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/NegativeFilter.h"
#include "filters/SharpeningFilter.h"
#include "io/BmpIO.h"

using imgproc::Color;
using imgproc::CropFilter;
using imgproc::EdgeDetectionFilter;
using imgproc::GaussianBlurFilter;
using imgproc::GrayscaleFilter;
using imgproc::Image;
using imgproc::KLuminanceRed;
using imgproc::KMaxColor;
using imgproc::KMidColor;
using imgproc::KMinColor;
using imgproc::LoadBmp24;
using imgproc::NegativeFilter;
using imgproc::SaveBmp24;
using imgproc::SharpeningFilter;

static int& GetPassed() {
    static int count = 0;
    return count;
}

static int& GetFailed() {
    static int count = 0;
    return count;
}

#define CHECK(cond)                                                                          \
    do {                                                                                     \
        if (!(cond)) {                                                                       \
            std::cerr << "FAIL  " << __func__ << "  line " << __LINE__ << "  (" #cond ")\n"; \
            ++GetFailed();                                                                   \
        } else {                                                                             \
            ++GetPassed();                                                                   \
        }                                                                                    \
    } while (false)

#define CHECK_NEAR(a, b, eps) CHECK(std::abs((a) - (b)) < (eps))

namespace {

constexpr double KEps = 1e-9;
constexpr double KEpsMedium = 1e-6;
constexpr double KEpsLarge = 1e-4;
constexpr double KLocalBmpTolerance = KMaxColor / imgproc::KByteScale + KEps;

constexpr int32_t KDim1 = 1;
constexpr int32_t KDim3 = 3;
constexpr int32_t KDim4 = 4;
constexpr int32_t KDim5 = 5;
constexpr int32_t KDim6 = 6;
constexpr int32_t KDim7 = 7;
constexpr int32_t KDim8 = 8;
constexpr int32_t KDim9 = 9;
constexpr int32_t KDim10 = 10;
constexpr int32_t KDim20 = 20;
constexpr int32_t KDim100 = 100;

constexpr int32_t KX0 = 0;
constexpr int32_t KX1 = 1;
constexpr int32_t KX2 = 2;
constexpr int32_t KX3 = 3;
constexpr int32_t KX5 = 5;

constexpr int32_t KNegOutOfBounds = -KDim5;
constexpr int32_t KPosOutOfBounds = KDim100;

constexpr double KColor01 = 0.1;
constexpr double KColor02 = 0.2;
constexpr double KColor025 = 0.25;
constexpr double KColor03 = 0.3;
constexpr double KColor04 = 0.4;
constexpr double KColor05 = 0.5;
constexpr double KColor06 = 0.6;
constexpr double KColor07 = 0.7;
constexpr double KColor075 = 0.75;
constexpr double KColor08 = 0.8;
constexpr double KColor09 = 0.9;

constexpr double KBlurSigma = 1.5;
constexpr double KNegativeSigma = -KMaxColor;
constexpr double KEdgeThreshold = 0.1;

constexpr char KProgramName[] = "image_processor";
constexpr char KInputName[] = "in.bmp";
constexpr char KOutputName[] = "out.bmp";
constexpr char KFilterCrop[] = "-crop";
constexpr char KFilterGs[] = "-gs";
constexpr char KFilterNeg[] = "-neg";
constexpr char KBadFilterToken[] = "crop";
constexpr char KCropWidthArg[] = "10";
constexpr char KCropHeightArg[] = "20";

Config ParseArgs(const std::vector<std::string>& args) {
    std::vector<std::string> storage = args;
    std::vector<char*> argv;
    argv.reserve(storage.size());
    for (std::string& arg : storage) {
        argv.push_back(arg.data());
    }
    return CommandLineParser::Parse(static_cast<int>(argv.size()), argv.data());
}

Image MakeSolid(int32_t w, int32_t h, double r, double g, double b) {
    Image img(w, h);
    for (int32_t y = 0; y < h; ++y) {
        for (int32_t x = 0; x < w; ++x) {
            img.At(x, y) = {r, g, b};
        }
    }
    return img;
}

void GetPixel(const Image& img, int32_t x, int32_t y, double& r, double& g, double& b) {
    const Color& c = img.At(x, y);
    r = c.r;
    g = c.g;
    b = c.b;
}

std::string TempPath(const std::string& name) {
    return "/tmp/" + name;
}

}  // namespace

void TestImageSize() {
    Image img(KDim10, KDim20);
    CHECK(img.Width() == KDim10);
    CHECK(img.Height() == KDim20);
}

void TestImageDefaultBlack() {
    Image img(KDim3, KDim3);
    double r = KMaxColor;
    double g = KMaxColor;
    double b = KMaxColor;
    GetPixel(img, KX1, KX1, r, g, b);
    CHECK_NEAR(r, KMinColor, KEps);
    CHECK_NEAR(g, KMinColor, KEps);
    CHECK_NEAR(b, KMinColor, KEps);
}

void TestImageSetGet() {
    Image img(KDim5, KDim5);
    img.At(KX2, KX3) = {KColor01, KColor04, KColor09};
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX2, KX3, r, g, b);
    CHECK_NEAR(r, KColor01, KEps);
    CHECK_NEAR(g, KColor04, KEps);
    CHECK_NEAR(b, KColor09, KEps);
}

void TestImageBorderClamping() {
    Image img(KDim4, KDim4);
    img.At(KX0, KX0) = {KMaxColor, KMinColor, KMinColor};
    const Color& a = img.AtClamped(KNegOutOfBounds, KNegOutOfBounds);
    const Color& b = img.AtClamped(KPosOutOfBounds, KPosOutOfBounds);
    CHECK_NEAR(a.r, KMaxColor, KEps);
    CHECK_NEAR(b.r, KMinColor, KEps);
}

void TestBmpRoundtrip() {
    const std::string path = TempPath("imgproc_roundtrip.bmp");
    Image src(KDim4, KDim4);
    src.At(KX1, KX2) = {KColor05, KColor025, KColor075};
    SaveBmp24(path, src);
    Image dst = LoadBmp24(path);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(dst, KX1, KX2, r, g, b);
    CHECK(dst.Width() == KDim4);
    CHECK(dst.Height() == KDim4);
    CHECK_NEAR(r, KColor05, KLocalBmpTolerance);
    CHECK_NEAR(g, KColor025, KLocalBmpTolerance);
    CHECK_NEAR(b, KColor075, KLocalBmpTolerance);
    std::remove(path.c_str());
}

void TestBmpUnsupportedSignatureThrows() {
    const std::string path = TempPath("imgproc_bad_signature.bmp");
    {
        std::ofstream out(path, std::ios::binary);
        out.put('N');
        out.put('O');
        out.put('P');
        out.put('E');
    }
    bool threw = false;
    try {
        static_cast<void>(LoadBmp24(path));
    } catch (const FormatError&) {
        threw = true;
    }
    CHECK(threw);
    std::remove(path.c_str());
}

void TestCropReducesSize() {
    Image img(KDim10, KDim10);
    CropFilter f(KDim6, KDim4);
    f.Apply(img);
    CHECK(img.Width() == KDim6);
    CHECK(img.Height() == KDim4);
}

void TestCropPreservesPixels() {
    Image img(KDim8, KDim8);
    img.At(KX2, KX3) = {KColor07, KColor02, KColor05};
    CropFilter f(KDim5, KDim5);
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX2, KX3, r, g, b);
    CHECK_NEAR(r, KColor07, KEps);
    CHECK_NEAR(g, KColor02, KEps);
    CHECK_NEAR(b, KColor05, KEps);
}

void TestCropLargerThanImageClampsToSource() {
    Image img(KDim5, KDim5);
    CropFilter f(KDim100, KDim100);
    f.Apply(img);
    CHECK(img.Width() == KDim5);
    CHECK(img.Height() == KDim5);
}

void TestGrayscaleWhiteStaysWhite() {
    Image img = MakeSolid(KDim4, KDim4, KMaxColor, KMaxColor, KMaxColor);
    GrayscaleFilter f;
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX2, KX2, r, g, b);
    CHECK_NEAR(r, KMaxColor, KEps);
    CHECK_NEAR(g, KMaxColor, KEps);
    CHECK_NEAR(b, KMaxColor, KEps);
}

void TestGrayscaleLuminosityFormula() {
    Image img(KDim1, KDim1);
    img.At(KX0, KX0) = {KMaxColor, KMinColor, KMinColor};
    GrayscaleFilter f;
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX0, KX0, r, g, b);
    CHECK_NEAR(r, KLuminanceRed, KEpsMedium);
    CHECK_NEAR(r, g, KEps);
    CHECK_NEAR(r, b, KEps);
}

void TestNegativeFormula() {
    Image img(KDim1, KDim1);
    img.At(KX0, KX0) = {KColor03, KColor06, KColor09};
    NegativeFilter f;
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX0, KX0, r, g, b);
    CHECK_NEAR(r, KColor07, KEps);
    CHECK_NEAR(g, KColor04, KEps);
    CHECK_NEAR(b, KColor01, KEps);
}

void TestNegativeDoubleIsIdentity() {
    Image img(KDim3, KDim3);
    img.At(KX1, KX1) = {KColor025, KColor05, KColor075};
    NegativeFilter f;
    f.Apply(img);
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX1, KX1, r, g, b);
    CHECK_NEAR(r, KColor025, KEps);
    CHECK_NEAR(g, KColor05, KEps);
    CHECK_NEAR(b, KColor075, KEps);
}

void TestSharpeningUniformImageUnchanged() {
    Image img = MakeSolid(KDim5, KDim5, KColor05, KColor05, KColor05);
    SharpeningFilter f;
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX2, KX2, r, g, b);
    CHECK_NEAR(r, KColor05, KEpsMedium);
    CHECK_NEAR(g, KColor05, KEpsMedium);
    CHECK_NEAR(b, KColor05, KEpsMedium);
}

void TestSharpeningOutputInRange() {
    Image img(KDim5, KDim5);
    img.At(KX2, KX2) = {KColor08, KColor03, KColor06};
    SharpeningFilter f;
    f.Apply(img);
    for (int32_t y = KX0; y < KDim5; ++y) {
        for (int32_t x = KX0; x < KDim5; ++x) {
            const Color& c = img.At(x, y);
            CHECK(c.r >= KMinColor && c.r <= KMaxColor);
            CHECK(c.g >= KMinColor && c.g <= KMaxColor);
            CHECK(c.b >= KMinColor && c.b <= KMaxColor);
        }
    }
}

void TestEdgeUniformImageAllBlack() {
    Image img = MakeSolid(KDim5, KDim5, KColor05, KColor05, KColor05);
    EdgeDetectionFilter f(KEdgeThreshold);
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX2, KX2, r, g, b);
    CHECK_NEAR(r, KMinColor, KEps);
    CHECK_NEAR(g, KMinColor, KEps);
    CHECK_NEAR(b, KMinColor, KEps);
}

void TestEdgeOutputOnlyBlackOrWhite() {
    Image img(KDim5, KDim5);
    img.At(KX2, KX2) = {KMaxColor, KMaxColor, KMaxColor};
    EdgeDetectionFilter f(KEdgeThreshold);
    f.Apply(img);
    for (int32_t y = KX0; y < KDim5; ++y) {
        for (int32_t x = KX0; x < KDim5; ++x) {
            const Color& c = img.At(x, y);
            const bool is_black = (std::abs(c.r) < KEps && std::abs(c.g) < KEps && std::abs(c.b) < KEps);
            const bool is_white = (std::abs(c.r - KMaxColor) < KEps && std::abs(c.g - KMaxColor) < KEps &&
                                   std::abs(c.b - KMaxColor) < KEps);
            CHECK(is_black || is_white);
        }
    }
}

void TestBlurUniformImageUnchanged() {
    Image img = MakeSolid(KDim7, KDim7, KColor06, KColor03, KColor08);
    GaussianBlurFilter f(KMaxColor);
    f.Apply(img);
    double r = KMinColor;
    double g = KMinColor;
    double b = KMinColor;
    GetPixel(img, KX3, KX3, r, g, b);
    CHECK_NEAR(r, KColor06, KEpsLarge);
    CHECK_NEAR(g, KColor03, KEpsLarge);
    CHECK_NEAR(b, KColor08, KEpsLarge);
}

void TestBlurNegativeSigmaThrows() {
    bool threw = false;
    try {
        GaussianBlurFilter f(KNegativeSigma);
        Image img(KDim4, KDim4);
        f.Apply(img);
    } catch (const ArgumentError&) {
        threw = true;
    }
    CHECK(threw);
}

void TestBlurSpreadsColor() {
    Image img = MakeSolid(KDim9, KDim9, KMinColor, KMinColor, KMinColor);
    img.At(KDim4, KDim4) = {KMaxColor, KMinColor, KMinColor};
    GaussianBlurFilter f(KBlurSigma);
    f.Apply(img);
    CHECK(img.At(KDim4, KX5).r > KMinColor);
}

void TestCreateFilterGs() {
    auto f = ::FilterFactory::Create("gs", {});
    CHECK(f != nullptr);
}

void TestCreateFilterCrop() {
    auto f = ::FilterFactory::Create("crop", {KCropWidthArg, KCropHeightArg});
    CHECK(f != nullptr);
}

void TestCreateFilterUnknownThrows() {
    bool threw = false;
    try {
        static_cast<void>(::FilterFactory::Create("xyz", {}));
    } catch (const ArgumentError&) {
        threw = true;
    }
    CHECK(threw);
}

void TestCreateFilterCropWrongParamsThrows() {
    bool threw = false;
    try {
        static_cast<void>(::FilterFactory::Create("crop", {KCropWidthArg}));
    } catch (const ArgumentError&) {
        threw = true;
    }
    CHECK(threw);
}

void TestParserBuildsPipeline() {
    Config cfg = ParseArgs(
        {KProgramName, KInputName, KOutputName, KFilterCrop, KCropWidthArg, KCropHeightArg, KFilterGs, KFilterNeg});
    CHECK(cfg.input_path == KInputName);
    CHECK(cfg.output_path == KOutputName);
    CHECK(cfg.filters.size() == static_cast<std::size_t>(KDim3));
    CHECK(cfg.filters[KX0]->Name() == "crop");
    CHECK(cfg.filters[KX1]->Name() == "gs");
    CHECK(cfg.filters[KX2]->Name() == "neg");
}

void TestParserBadTokenThrows() {
    bool threw = false;
    try {
        static_cast<void>(ParseArgs({KProgramName, KInputName, KOutputName, KBadFilterToken}));
    } catch (const ArgumentError&) {
        threw = true;
    }
    CHECK(threw);
}

#define RUN(fn)                     \
    do {                            \
        fn();                       \
        std::cout << "  " #fn "\n"; \
    } while (false)

int main() {
    std::cout << "=== Image ===\n";
    RUN(TestImageSize);
    RUN(TestImageDefaultBlack);
    RUN(TestImageSetGet);
    RUN(TestImageBorderClamping);

    std::cout << "=== BMP I/O ===\n";
    RUN(TestBmpRoundtrip);
    RUN(TestBmpUnsupportedSignatureThrows);

    std::cout << "=== CropFilter ===\n";
    RUN(TestCropReducesSize);
    RUN(TestCropPreservesPixels);
    RUN(TestCropLargerThanImageClampsToSource);

    std::cout << "=== GrayscaleFilter ===\n";
    RUN(TestGrayscaleWhiteStaysWhite);
    RUN(TestGrayscaleLuminosityFormula);

    std::cout << "=== NegativeFilter ===\n";
    RUN(TestNegativeFormula);
    RUN(TestNegativeDoubleIsIdentity);

    std::cout << "=== SharpeningFilter ===\n";
    RUN(TestSharpeningUniformImageUnchanged);
    RUN(TestSharpeningOutputInRange);

    std::cout << "=== EdgeDetectionFilter ===\n";
    RUN(TestEdgeUniformImageAllBlack);
    RUN(TestEdgeOutputOnlyBlackOrWhite);

    std::cout << "=== GaussianBlurFilter ===\n";
    RUN(TestBlurUniformImageUnchanged);
    RUN(TestBlurNegativeSigmaThrows);
    RUN(TestBlurSpreadsColor);

    std::cout << "=== FilterFactory ===\n";
    RUN(TestCreateFilterGs);
    RUN(TestCreateFilterCrop);
    RUN(TestCreateFilterUnknownThrows);
    RUN(TestCreateFilterCropWrongParamsThrows);

    std::cout << "=== CommandLineParser ===\n";
    RUN(TestParserBuildsPipeline);
    RUN(TestParserBadTokenThrows);

    std::cout << "\nResults: " << GetPassed() << " passed, " << GetFailed() << " failed\n";
    return GetFailed() == 0 ? 0 : 1;
}
