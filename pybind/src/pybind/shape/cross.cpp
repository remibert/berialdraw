#include "pybind/pyberialdraw.hpp"
void bind_cross(py::module& m) {
    py::class_<berialdraw::Cross, berialdraw::Marker, std::unique_ptr<berialdraw::Cross, py::nodelete>>(m, "Cross")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Cross&>(),
             py::arg("other"),
             "Copy constructor");
}
