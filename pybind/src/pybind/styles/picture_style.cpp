#include "pybind/pyberialdraw.hpp"

void bind_picture_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::PictureStyle, berialdraw::Style> cls(m, "PictureStyle");
    cls.def(pybind11::init<>(), "Constructor");

    // filename property
    cls.def_property("filename",
        [](berialdraw::PictureStyle& self) -> std::string {
            return std::string(self.filename().c_str());
        },
        [](berialdraw::PictureStyle& self, const std::string& value) {
            self.filename(value.c_str());
        },
        "Image filename (PNG or JPEG)");

    // alpha property
    cls.def_property("alpha",
        [](berialdraw::PictureStyle& self) -> int {
            return (int)self.alpha();
        },
        [](berialdraw::PictureStyle& self, int value) {
            if (value < 0) value = 0;
            if (value > 255) value = 255;
            self.alpha((uint8_t)value);
        },
        "Alpha transparency (0=transparent, 255=opaque)");

    // fit_mode property
    cls.def_property("fit_mode",
        [](berialdraw::PictureStyle& self) -> int {
            return (int)self.fit_mode();
        },
        [](berialdraw::PictureStyle& self, int value) {
            self.fit_mode((berialdraw::ImageFitMode)value);
        },
        "Image fit mode (0=stretch, 1=aspect_width, 2=aspect_height, 3=aspect_fit, 4=aspect_fill, 5=none)");
}
