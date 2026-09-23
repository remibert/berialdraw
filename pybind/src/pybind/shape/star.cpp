#include "pybind/pyberialdraw.hpp"
void bind_star(py::module& m) {
    py::class_<berialdraw::Star, berialdraw::Marker, std::unique_ptr<berialdraw::Star, py::nodelete>>(m, "Star")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Star&>(),
             py::arg("other"),
             "Copy constructor");
}
