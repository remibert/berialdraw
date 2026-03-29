#include "pybind/pyberialdraw.hpp"

void bind_grid_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::GridStyle, berialdraw::Style> cls(m, "GridStyle");
    cls.def(pybind11::init<>())
        // Grid color property
        .def_property("grid_color",
            [](berialdraw::GridStyle& self) -> uint32_t { return self.grid_color(); },
            [](berialdraw::GridStyle& self, uint32_t col) { self.grid_color(col); },
            "Grid color")
        // Grid visibility property
        .def_property("grid_visible",
            [](berialdraw::GridStyle& self) -> bool { return self.grid_visible(); },
            [](berialdraw::GridStyle& self, bool visible) { self.grid_visible(visible); },
            "Grid visibility state")
        // Horizontal thickness property
        .def_property("horizontal_thickness",
            [](berialdraw::GridStyle& self) -> berialdraw::Dim { return self.horizontal_thickness(); },
            [](berialdraw::GridStyle& self, berialdraw::Dim value) { self.horizontal_thickness(value); },
            "Horizontal grid thickness in pixels")
        // Vertical thickness property
        .def_property("vertical_thickness",
            [](berialdraw::GridStyle& self) -> berialdraw::Dim { return self.vertical_thickness(); },
            [](berialdraw::GridStyle& self, berialdraw::Dim value) { self.vertical_thickness(value); },
            "Vertical grid thickness in pixels");
}
