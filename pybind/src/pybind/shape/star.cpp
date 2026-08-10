#include "pybind/pyberialdraw.hpp"
void bind_star(py::module& m) {
    py::class_<berialdraw::Star, berialdraw::Marker>(m, "Star")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor");
}
