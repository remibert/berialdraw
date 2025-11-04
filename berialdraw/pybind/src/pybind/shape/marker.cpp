#include "pybind/pyberialdraw.hpp"
void bind_marker(py::module& m) {
    py::class_<berialdraw::Marker, berialdraw::RoundStyle>(m, "Marker")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Create a marker")
        .def(py::init<const berialdraw::Marker&>(),
             py::arg("other"),
             "Copy constructor");
}
