#include "pybind/pyberialdraw.hpp"
void bind_triangle(py::module& m) {
    py::class_<berialdraw::Triangle, berialdraw::Marker, std::unique_ptr<berialdraw::Triangle, py::nodelete>>(m, "Triangle")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for Triangle")
        .def(py::init<const berialdraw::Triangle&>(),
             py::arg("other"),
             "Copy constructor");
}
