#include "pybind/pyberialdraw.hpp"
void bind_circle(py::module& m) {
    py::class_<berialdraw::Circle, berialdraw::Marker>(m, "Circle")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor");
}
