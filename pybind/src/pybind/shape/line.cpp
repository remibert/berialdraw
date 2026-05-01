#include "pybind/pyberialdraw.hpp"
void bind_line(py::module& m) {
    py::class_<berialdraw::Line, berialdraw::LineStyle, berialdraw::RoundStyle>(m, "Line")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Line&>(),
             py::arg("other"),
             "Copy constructor");
}
