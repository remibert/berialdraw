#include "pybind/pyberialdraw.hpp"

void bind_table_view_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::TableViewStyle, berialdraw::Style> cls(m, "TableViewStyle");
    cls.def(pybind11::init<>())
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
