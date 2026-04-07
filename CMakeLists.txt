cmake_minimum_required(VERSION 3.16)
project(image_processor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(image_processor_lib
    src/app/Application.cpp
    src/app/CommandLineParser.cpp
    src/app/FilterFactory.cpp
    src/app/HelpPrinter.cpp

    src/core/Image.cpp

    src/io/BmpIO.cpp

    src/filters/PointwiseFilter.cpp
    src/filters/MatrixFilter.cpp
    src/filters/CropFilter.cpp
    src/filters/GrayscaleFilter.cpp
    src/filters/NegativeFilter.cpp
    src/filters/SharpeningFilter.cpp
    src/filters/EdgeDetectionFilter.cpp
    src/filters/GaussianBlurFilter.cpp
    src/filters/ArtisticLensFilter.cpp
)

target_include_directories(image_processor_lib PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

add_executable(image_processor src/main.cpp)
target_link_libraries(image_processor PRIVATE image_processor_lib)

include(CTest)
if(BUILD_TESTING)
    add_executable(unit_tests tests/test_image_processor.cpp)
    target_link_libraries(unit_tests PRIVATE image_processor_lib)
    add_test(NAME unit_tests COMMAND unit_tests)
endif()
