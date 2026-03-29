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
            "Column selector");
}
