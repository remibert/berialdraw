#include "pybind/pyberialdraw.hpp"

void bind_grid_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::GridStyle, berialdraw::Style> cls(m, "GridStyle");
    cls.def(pybind11::init<>())
        // Grid color property
        .def_property(berialdraw::StyleNames::GRIDSTYLE_GRID_COLOR,
            [](berialdraw::GridStyle& self) -> uint32_t { return self.grid_color(); },
            [](berialdraw::GridStyle& self, uint32_t col) { self.grid_color(col); },
            "Grid color")
        // Grid visibility property
        .def_property(berialdraw::StyleNames::GRIDSTYLE_GRID_VISIBLE,
            [](berialdraw::GridStyle& self) -> bool { return self.grid_visible(); },
            [](berialdraw::GridStyle& self, bool visible) { self.grid_visible(visible); },
            "Grid visibility state");

    bind_precision_property<berialdraw::GridStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::GRIDSTYLE_HORIZONTAL_THICKNESS,
        &berialdraw::GridStyle::horizontal_thickness,
        &berialdraw::GridStyle::horizontal_thickness,
        &berialdraw::GridStyle::horizontal_thickness_q6,
        "Horizontal grid thickness (int for normal, float for high precision)");
    bind_precision_property<berialdraw::GridStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::GRIDSTYLE_VERTICAL_THICKNESS,
        &berialdraw::GridStyle::vertical_thickness,
        &berialdraw::GridStyle::vertical_thickness,
        &berialdraw::GridStyle::vertical_thickness_q6,
        "Vertical grid thickness (int for normal, float for high precision)");
}
