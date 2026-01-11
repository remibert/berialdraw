#include "pybind/pyberialdraw.hpp"
void bind_border_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::BorderStyle, berialdraw::Style> cls(m, "BorderStyle");
    cls.def(pybind11::init<>())
        .def_property("radius",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.radius(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.radius(value); }, "Border radius")
        .def_property("thickness",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.thickness(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.thickness(value); }, "Border thickness");
    
    bind_color_property(cls, "border_color",
        &berialdraw::BorderStyle::border_color,
        static_cast<void (berialdraw::BorderStyle::*)(uint32_t)>(&berialdraw::BorderStyle::border_color),
        "Border color");
    bind_color_property(cls, "focus_color",
        &berialdraw::BorderStyle::focus_color,
        static_cast<void (berialdraw::BorderStyle::*)(uint32_t)>(&berialdraw::BorderStyle::focus_color),
        "Focus color");
    
    cls.def_property("focus_gap",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.focus_gap(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.focus_gap(value); }, "Focus gap")
        .def_property("focus_thickness",
            [](berialdraw::BorderStyle& self) -> berialdraw::Dim { return self.focus_thickness(); },
            [](berialdraw::BorderStyle& self, berialdraw::Dim value) { self.focus_thickness(value); }, "Focus thickness");
}
