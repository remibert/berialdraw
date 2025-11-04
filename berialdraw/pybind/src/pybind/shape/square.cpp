#include "pybind/pyberialdraw.hpp"
void bind_square(py::module& m) {
    py::class_<berialdraw::Square, berialdraw::Marker>(m, "Square")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for Square");
}
