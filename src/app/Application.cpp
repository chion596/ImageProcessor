#include "app/Application.h"

#include "io/BmpIO.h"

void Application::Run(const Config& config) const {
    imgproc::Image image = imgproc::LoadBmp24(config.input_path);

    for (const auto& filter : config.filters) {
        filter->Apply(image);
    }

    imgproc::SaveBmp24(config.output_path, image);
}
