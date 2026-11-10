#include "pybind/pyberialdraw.hpp"

void bind_grid_shape(py::module& m)
{
    py::class_<berialdraw::GridShape, berialdraw::Shape, std::unique_ptr<berialdraw::GridShape, py::nodelete>>(m, "GridShape")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Create a regular grid shape on a canvas")
        .def_property("rows",
            [](const berialdraw::GridShape& self) { return self.rows(); },
            [](berialdraw::GridShape& self, berialdraw::Dim count) { self.rows(count); })
        .def_property("columns",
            [](const berialdraw::GridShape& self) { return self.columns(); },
            [](berialdraw::GridShape& self, berialdraw::Dim count) { self.columns(count); })
        .def_property("grid_color",
            [](const berialdraw::GridShape& self) { return self.grid_color(); },
            [](berialdraw::GridShape& self, uint32_t color) { self.grid_color(color); })
        .def_property("grid_visible",
            [](const berialdraw::GridShape& self) { return self.grid_visible(); },
            [](berialdraw::GridShape& self, bool visible) { self.grid_visible(visible); })
        .def_property("horizontal_thickness",
            [](const berialdraw::GridShape& self) { return self.horizontal_thickness(); },
            [](berialdraw::GridShape& self, berialdraw::Dim thickness) { self.horizontal_thickness(thickness); })
        .def_property("vertical_thickness",
            [](const berialdraw::GridShape& self) { return self.vertical_thickness(); },
            [](berialdraw::GridShape& self, berialdraw::Dim thickness) { self.vertical_thickness(thickness); });
}
