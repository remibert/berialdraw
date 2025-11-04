#include "pybind/pyberialdraw.hpp"
void bind_border_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::BorderStyle, berialdraw::Style>(m, "BorderStyle")
        .def(pybind11::init<>())

        .def_property("radius",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.radius(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.radius(value); }, "Border radius")
        .def_property("thickness",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.thickness(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.thickness(value); }, "Border thickness")
        .def_property("border_color",
            [](berialdraw::BorderStyle& self) -> uint32_t { return self.border_color(); },
            [](berialdraw::BorderStyle& self, uint32_t value) { self.border_color(value); }, "Border color")
        .def_property("focus_color",
            [](berialdraw::BorderStyle& self) -> uint32_t { return self.focus_color(); },
            [](berialdraw::BorderStyle& self, uint32_t value) { self.focus_color(value); }, "Focus color")
        .def_property("focus_gap",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.focus_gap(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.focus_gap(value); }, "Focus gap")
        .def_property("focus_thickness",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.focus_thickness(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.focus_thickness(value); }, "Focus thickness");
}
