#pragma once

#include <stdexcept>
#include <string>

class ImageProcessorError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ArgumentError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};

class IoError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};

class FormatError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};
