#include "pybind/pyberialdraw.hpp"

void bind_font(py::module& m) {
    py::enum_<berialdraw::Font::Style>(m, "FontStyle")
        .value("UNDEFINED", berialdraw::Font::Style::UNDEFINED, "Undefined style")
        .value("NORMAL", berialdraw::Font::Style::NORMAL, "Normal style")
        .value("ITALIC", berialdraw::Font::Style::ITALIC, "Italic style")
        .value("BOLD", berialdraw::Font::Style::BOLD, "Bold style")
        .value("BOLD_ITALIC", berialdraw::Font::Style::BOLD_ITALIC, "Bold and Italic style");
}
