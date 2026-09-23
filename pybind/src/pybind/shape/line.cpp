#include "pybind/pyberialdraw.hpp"
void bind_line(py::module& m) {
    py::class_<berialdraw::Line, berialdraw::Shape, berialdraw::LineStyle, berialdraw::RoundStyle, std::unique_ptr<berialdraw::Line, py::nodelete>>(m, "Line")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Line&>(),
             py::arg("other"),
             "Copy constructor");
}
