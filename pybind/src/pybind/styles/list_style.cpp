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
            }, "Separator thickness in pixels")
        .def_property("show_arrow",
            [](berialdraw::ListStyle& self) -> bool { 
                return self.show_arrow(); 
            },
            [](berialdraw::ListStyle& self, bool v) { 
                self.show_arrow(v); 
            }, "Whether to show arrow indicator")
        .def_property("arrow_right",
            [](berialdraw::ListStyle& self) -> bool { 
                return self.arrow_right(); 
            },
            [](berialdraw::ListStyle& self, bool v) { 
                self.arrow_right(v); 
            }, "Arrow direction (true = right, false = left)")
        .def_property("disabled_text_color",
            [](berialdraw::ListStyle& self) -> uint32_t { 
                return self.disabled_text_color(); 
            },
            [](berialdraw::ListStyle& self, uint32_t c) { 
                self.disabled_text_color(c); 
            }, "Disabled item text color");
}
