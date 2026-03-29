#include "pybind/pyberialdraw.hpp"
void bind_area(py::module& m) {
    py::class_<berialdraw::Area> cls(m, "Area");

    cls.def(py::init<const berialdraw::Point&, const berialdraw::Size&>(),
             py::arg("position"), py::arg("size"),
             "Create area with position and size")
        .def(py::init<berialdraw::Coord, berialdraw::Coord, berialdraw::Dim, berialdraw::Dim, bool>(),
             py::arg("x") = 0, py::arg("y") = 0, py::arg("w") = 0, py::arg("h") = 0, py::arg("pixel") = true,
             "Create area with coordinates")
        .def(py::init<const berialdraw::Area&>(),
             py::arg("other"),
             "Create a copy of area")
        .def("move", &berialdraw::Area::move,
             py::arg("shift"),
             "Move the point by specified coordinates with a precision of 64th of a pixel")
        .def("set", &berialdraw::Area::set,
             py::arg("position"), py::arg("size"),
             "Set the position and size of the area")
        .def("nearest_pixel", &berialdraw::Area::nearest_pixel,
             "Resizes and move itself on the nearest pixel")
        .def("decrease", &berialdraw::Area::decrease,
             py::arg("margin"),
             "Decrease size and position of area with margin")
        .def("increase", &berialdraw::Area::increase,
             py::arg("margin"),
             "Increase size and position of area with margin")
        .def("print", &berialdraw::Area::print,
             py::arg("name"),
             "Print content");

    // Propriétés avec précision automatique (int ou float*64)
    bind_precision_property(cls, "width",
        &berialdraw::Area::width,
        &berialdraw::Area::width,
        &berialdraw::Area::width_,
        "Width (int for normal, float for high precision)");
    
    bind_precision_property(cls, "height",
        &berialdraw::Area::height,
        &berialdraw::Area::height,
        &berialdraw::Area::height_,
        "Height (int for normal, float for high precision)");
    
    bind_precision_property(cls, "x",
        &berialdraw::Area::x,
        &berialdraw::Area::x,
        &berialdraw::Area::x_,
        "X coordinate (int for normal, float for high precision)");
    
    bind_precision_property(cls, "y",
        &berialdraw::Area::y,
        &berialdraw::Area::y,
        &berialdraw::Area::y_,
        "Y coordinate (int for normal, float for high precision)");

    // Position and size properties
    bind_point_from_values_property(cls, "position",
        &berialdraw::Area::position,
        static_cast<void (berialdraw::Area::*)(const berialdraw::Point&)>(&berialdraw::Area::position),
        "Position as (x, y) tuple");

    bind_size_from_values_property(cls, "size",
        &berialdraw::Area::size,
        static_cast<void (berialdraw::Area::*)(const berialdraw::Size&)>(&berialdraw::Area::size),
        "Size as (width, height) tuple");
}
