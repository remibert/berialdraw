#include "pybind/pyberialdraw.hpp"
void bind_scroll_direction(py::module& m) {
    py::enum_<berialdraw::ScrollDirection>(m, "ScrollDirection")
        .value("SCROLL_HORIZONTAL", berialdraw::SCROLL_HORIZONTAL, "Scroll horizontally")
        .value("SCROLL_VERTICAL", berialdraw::SCROLL_VERTICAL, "Scroll vertically")
        .value("SCROLL_ALL_DIRECTIONS", berialdraw::SCROLL_ALL_DIRECTIONS, "Scroll in all directions");
}
