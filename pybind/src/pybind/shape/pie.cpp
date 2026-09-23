#include "pybind/pyberialdraw.hpp"
void bind_pie(py::module& m) {
    py::class_<berialdraw::Pie, berialdraw::Shape, berialdraw::PieStyle, berialdraw::RoundStyle, std::unique_ptr<berialdraw::Pie, py::nodelete>>(m, "Pie")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for Pie")
        .def(py::init<const berialdraw::Pie&>(),
             py::arg("other"),
             "Copy constructor for Pie");
}
