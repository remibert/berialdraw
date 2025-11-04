#include "pybind/pyberialdraw.hpp"
void bind_cross(py::module& m) {
    py::class_<berialdraw::Cross, berialdraw::Marker>(m, "Cross")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor");
}
