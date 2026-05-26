#include "pybind/pyberialdraw.hpp"
void bind_image_fit_mode(py::module& m) {
    py::enum_<berialdraw::ImageFitMode>(m, "ImageFitMode")
        .value("IMAGE_FIT_STRETCH", berialdraw::IMAGE_FIT_STRETCH, "Stretch the image to fill the entire area (ignores aspect ratio)")
        .value("IMAGE_FIT_ASPECT_WIDTH", berialdraw::IMAGE_FIT_ASPECT_WIDTH, "Preserve aspect ratio, fit to area width")
        .value("IMAGE_FIT_ASPECT_HEIGHT", berialdraw::IMAGE_FIT_ASPECT_HEIGHT, "Preserve aspect ratio, fit to area height")
        .value("IMAGE_FIT_ASPECT_FIT", berialdraw::IMAGE_FIT_ASPECT_FIT, "Preserve aspect ratio, fit entirely inside area (may leave borders)")
        .value("IMAGE_FIT_ASPECT_FILL", berialdraw::IMAGE_FIT_ASPECT_FILL, "Preserve aspect ratio, fill entire area (may crop)")
        .value("IMAGE_FIT_NONE", berialdraw::IMAGE_FIT_NONE, "Display at native size, no scaling");
}
