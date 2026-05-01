#include "pybind/pyberialdraw.hpp"
void bind_point(py::module& m) {
    auto cls = py::class_<berialdraw::Point>(m, "Point")
        .def(py::init<>(), "Constructor")
        .def(py::init<berialdraw::Coord, berialdraw::Coord, bool>(),
             py::arg("x"), py::arg("y"), py::arg("pixel") = true,
             "Constructor with individual coordinates")
        .def(py::init<const berialdraw::Point&>(),
             py::arg("p"),
             "Copy constructor")
        .def(py::init<const berialdraw::Size&>(),
             py::arg("s"),
             "Constructor with size")
        .def("__eq__", &berialdraw::Point::operator==,
             py::arg("other"),
             "Equality operator")
        .def("__ne__", &berialdraw::Point::operator!=,
             py::arg("other"),
             "Inequality operator")
        .def("move", py::overload_cast<const berialdraw::Point&>(&berialdraw::Point::move),
             py::arg("p"),
             "Move the point by another point")
        .def("move", py::overload_cast<berialdraw::Coord, berialdraw::Coord>(&berialdraw::Point::move),
             py::arg("x_"), py::arg("y_"),
             "Move the point by specified coordinates")
        .def("set", &berialdraw::Point::set,
             py::arg("x"), py::arg("y"),
             "Set the coordinates")
        .def("set_", &berialdraw::Point::set_,
             py::arg("x"), py::arg("y"),
             "Set the coordinates with a precision of 64th of a pixel")
        .def("move_", &berialdraw::Point::move_,
             py::arg("x_"), py::arg("y_"),
             "Move the point with a precision of 64th of a pixel")
        .def("nearest_pixel", &berialdraw::Point::nearest_pixel,
             "Positions itself on the nearest pixel")
        .def("is_x_undefined", &berialdraw::Point::is_x_undefined,
             "Indicates if x is not defined")
        .def("is_y_undefined", &berialdraw::Point::is_y_undefined,
             "Indicates if y is not defined")
        .def("print", &berialdraw::Point::print,
             py::arg("name"),
             "Print content")
        .def("adapt_scale", &berialdraw::Point::adapt_scale,
             "Adapt point to the UIManager scale");
             
    // Propriétés avec précision automatique
    bind_precision_property(cls, "x",
        &berialdraw::Point::x,
        &berialdraw::Point::x,
        &berialdraw::Point::x_,
        "X coordinate (int for normal, float for high precision)");
    bind_precision_property(cls, "y",
        &berialdraw::Point::y,
        &berialdraw::Point::y,
        &berialdraw::Point::y_,
        "Y coordinate (int for normal, float for high precision)");
}
