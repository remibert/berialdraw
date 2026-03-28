#include "pybind/pyberialdraw.hpp"

void bind_cell_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::CellStyle, berialdraw::Style> cls(m, "CellStyle");
    cls.def(pybind11::init<>())
        // Row selector property
        .def_property("row_selector",
            [](berialdraw::CellStyle& self) -> uint16_t { return self.row_selector(); },
            [](berialdraw::CellStyle& self, uint16_t row) { self.row_selector(row); },
            "Row selector")
        // Column selector property
        .def_property("column_selector",
            [](berialdraw::CellStyle& self) -> uint16_t { return self.column_selector(); },
            [](berialdraw::CellStyle& self, uint16_t col) { self.column_selector(col); },
            "Column selector")
        // Grid color property
        .def_property("grid_color",
            [](berialdraw::CellStyle& self) -> uint32_t { return self.grid_color(); },
            [](berialdraw::CellStyle& self, uint32_t col) { self.grid_color(col); },
            "Grid color")
        // Grid visibility property
        .def_property("grid_visible",
            [](berialdraw::CellStyle& self) -> bool { return self.grid_visible(); },
            [](berialdraw::CellStyle& self, bool visible) { self.grid_visible(visible); },
            "Grid visibility state")
        // Horizontal thickness property
        .def_property("horizontal_thickness",
            [](berialdraw::CellStyle& self) -> berialdraw::Dim { return self.horizontal_thickness(); },
            [](berialdraw::CellStyle& self, berialdraw::Dim value) { self.horizontal_thickness(value); },
            "Horizontal grid thickness in pixels")
        // Vertical thickness property
        .def_property("vertical_thickness",
            [](berialdraw::CellStyle& self) -> berialdraw::Dim { return self.vertical_thickness(); },
            [](berialdraw::CellStyle& self, berialdraw::Dim value) { self.vertical_thickness(value); },
            "Vertical grid thickness in pixels");
}
