#include "pybind/pyberialdraw.hpp"
void bind_scroll_view_style(py::module& m) {
    // Bind ScrollDirection enum first
    py::enum_<berialdraw::ScrollDirection>(m, "ScrollDirection")
        .value("ScrollHorizontal", berialdraw::ScrollHorizontal)
        .value("ScrollVertical", berialdraw::ScrollVertical)
        .value("ScrollAllDirections", berialdraw::ScrollAllDirections);

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
