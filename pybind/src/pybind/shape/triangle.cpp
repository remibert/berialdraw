#include "pybind/pyberialdraw.hpp"
void bind_triangle(py::module& m) {
    py::class_<berialdraw::Triangle, berialdraw::Marker>(m, "Triangle")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for Triangle");
}
