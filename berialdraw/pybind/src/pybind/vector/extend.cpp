#include "pybind/pyberialdraw.hpp"
void bind_extend(py::module& m) {
    py::enum_<berialdraw::Extend>(m, "Extend")
        .value("EXTEND_NONE", berialdraw::EXTEND_NONE, "No extension in any direction")
        .value("EXTEND_WIDTH", berialdraw::EXTEND_WIDTH, "Extend to fill available width")
        .value("EXTEND_HEIGHT", berialdraw::EXTEND_HEIGHT, "Extend to fill available height")
        .value("EXTEND_ALL", berialdraw::EXTEND_ALL, "Extend to fill all available space");
}
