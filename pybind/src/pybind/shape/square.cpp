#include "pybind/pyberialdraw.hpp"
void bind_square(py::module& m) {
    py::class_<berialdraw::Square, berialdraw::Marker, std::unique_ptr<berialdraw::Square, py::nodelete>>(m, "Square")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for Square")
        .def(py::init<const berialdraw::Square&>(),
             py::arg("other"),
             "Copy constructor");
}
