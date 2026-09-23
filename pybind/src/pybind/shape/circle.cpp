#include "pybind/pyberialdraw.hpp"
void bind_circle(py::module& m) {
    py::class_<berialdraw::Circle, berialdraw::Marker, std::unique_ptr<berialdraw::Circle, py::nodelete>>(m, "Circle")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Circle&>(),
             py::arg("other"),
             "Copy constructor");
}
