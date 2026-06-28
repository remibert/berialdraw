#include "pybind/pyberialdraw.hpp"

void bind_list_style(py::module& m) {
    py::class_<berialdraw::ListStyle, berialdraw::Style>(m, "ListStyle")
        .def(py::init<>(), "Constructor")
        .def_property("separator_color",
            [](berialdraw::ListStyle& self) -> uint32_t { 
                return self.separator_color(); 
            },
            [](berialdraw::ListStyle& self, uint32_t c) { 
                self.separator_color(c); 
            }, "Separator color")
        .def_property("separator_thickness",
            [](berialdraw::ListStyle& self) -> berialdraw::Dim { 
                return self.separator_thickness(); 
            },
            [](berialdraw::ListStyle& self, berialdraw::Dim t) { 
                self.separator_thickness(t); 
            }, "Separator thickness in pixels");
}
