#include "pybind/pyberialdraw.hpp"
void bind_rect(py::module& m) {
    py::class_<berialdraw::Rect, berialdraw::RoundStyle>(m, "Rect")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Rect&>(),
             py::arg("other"),
             "Copy constructor");
}
