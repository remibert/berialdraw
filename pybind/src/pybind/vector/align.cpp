#include "pybind/pyberialdraw.hpp"
void bind_align(py::module& m) {
    py::enum_<berialdraw::Align>(m, "Align")
        .value("CENTER", berialdraw::Align::CENTER, "Placed in the center")
        .value("ALIGN_LEFT", berialdraw::Align::ALIGN_LEFT, "Placed in horizontal on the left")
        .value("ALIGN_RIGHT", berialdraw::Align::ALIGN_RIGHT, "Placed in horizontal on the right")
        .value("ALIGN_TOP", berialdraw::Align::ALIGN_TOP, "Placed in vertical on the top")
        .value("ALIGN_BOTTOM", berialdraw::Align::ALIGN_BOTTOM, "Placed in vertical on the bottom")
        .value("ALIGN_TOP_LEFT", berialdraw::Align::ALIGN_TOP_LEFT, "Placed on the top left corner")
        .value("ALIGN_TOP_RIGHT", berialdraw::Align::ALIGN_TOP_RIGHT, "Placed on the top right corner")
        .value("ALIGN_BOTTOM_LEFT", berialdraw::Align::ALIGN_BOTTOM_LEFT, "Placed on the bottom left corner")
        .value("ALIGN_BOTTOM_RIGHT", berialdraw::Align::ALIGN_BOTTOM_RIGHT, "Placed on the bottom right corner")
        .value("ALIGN_HORIZONTAL", berialdraw::Align::ALIGN_HORIZONTAL, "Placed horizontal center")
        .value("ALIGN_VERTICAL", berialdraw::Align::ALIGN_VERTICAL, "Placed vertical center")
        .value("ALIGN_DEFAULT", berialdraw::Align::ALIGN_DEFAULT, "Not initialized (Center)");
}
