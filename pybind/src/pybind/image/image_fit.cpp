#include "pybind/pyberialdraw.hpp"
void bind_image_fit_mode(py::module& m) {
    py::enum_<berialdraw::ImageFitMode>(m, "ImageFitMode")
        .value("STRETCH", berialdraw::STRETCH, "Stretch the image to fill the entire area (ignores aspect ratio)")
        .value("FIT", berialdraw::FIT, "Preserve aspect ratio, fit to available area");
}
