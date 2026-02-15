#include "pybind/pyberialdraw.hpp"

void bind_table_view_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::TableViewStyle, berialdraw::Style> cls(m, "TableViewStyle");
    cls.def(pybind11::init<>())
        // Grid color property
        .def_property("grid_color",
            [](berialdraw::TableViewStyle& self) -> uint32_t { return self.grid_color(); },
            [](berialdraw::TableViewStyle& self, uint32_t col) { self.grid_color(col); },
            "Grid color")
        // Horizontal thickness property
        .def_property("horizontal_thickness",
            [](berialdraw::TableViewStyle& self) -> berialdraw::Dim { return self.horizontal_thickness(); },
            [](berialdraw::TableViewStyle& self, berialdraw::Dim value) { self.horizontal_thickness(value); },
            "Horizontal grid thickness in pixels")
        // Vertical thickness property
        .def_property("vertical_thickness",
            [](berialdraw::TableViewStyle& self) -> berialdraw::Dim { return self.vertical_thickness(); },
            [](berialdraw::TableViewStyle& self, berialdraw::Dim value) { self.vertical_thickness(value); },
            "Vertical grid thickness in pixels")
        // Grid visibility property
        .def_property("grid_visible",
            [](berialdraw::TableViewStyle& self) -> bool { return self.grid_visible(); },
            [](berialdraw::TableViewStyle& self, bool visible) { self.grid_visible(visible); },
            "Grid visibility state")
        // Header visibility property
        .def_property("header_visible",
            [](berialdraw::TableViewStyle& self) -> bool { return self.header_visible(); },
            [](berialdraw::TableViewStyle& self, bool visible) { self.header_visible(visible); },
            "Header visibility state")
        // Header background color property
        .def_property("header_background_color",
            [](berialdraw::TableViewStyle& self) -> uint32_t { return self.header_background_color(); },
            [](berialdraw::TableViewStyle& self, uint32_t col) { self.header_background_color(col); },
            "Header background color")
        // Alternating row color 1 property
        .def_property("alternating_row_color1",
            [](berialdraw::TableViewStyle& self) -> uint32_t { return self.alternating_row_color1(); },
            [](berialdraw::TableViewStyle& self, uint32_t col) { self.alternating_row_color1(col); },
            "Alternating row color 1")
        // Alternating row color 2 property
        .def_property("alternating_row_color2",
            [](berialdraw::TableViewStyle& self) -> uint32_t { return self.alternating_row_color2(); },
            [](berialdraw::TableViewStyle& self, uint32_t col) { self.alternating_row_color2(col); },
            "Alternating row color 2");
}
