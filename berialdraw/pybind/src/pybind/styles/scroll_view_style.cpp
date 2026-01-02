#include "pybind/pyberialdraw.hpp"
void bind_scroll_view_style(py::module& m) {
    // Bind ScrollDirection enum first
    py::enum_<berialdraw::ScrollDirection>(m, "ScrollDirection")
        .value("SCROLL_HORIZONTAL", berialdraw::SCROLL_HORIZONTAL)
        .value("SCROLL_VERTICAL", berialdraw::SCROLL_VERTICAL)
        .value("SCROLL_ALL_DIRECTIONS", berialdraw::SCROLL_ALL_DIRECTIONS);

    py::class_<berialdraw::ScrollViewStyle, berialdraw::Style>(m, "ScrollViewStyle")
        .def(py::init<>(), "Constructor")
        .def_property("scroll_direction",
            [](berialdraw::ScrollViewStyle& self) -> berialdraw::ScrollDirection { 
                return self.scroll_direction(); 
            },
            [](berialdraw::ScrollViewStyle& self, berialdraw::ScrollDirection dir) { 
                self.scroll_direction(dir); 
            }, "Scroll direction");
}
