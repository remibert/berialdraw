#include "pybind/pyberialdraw.hpp"

void bind_styles(py::module& m) {
    py::class_<berialdraw::Styles>(m, "Styles")
        .def("clear", &berialdraw::Styles::clear,
             "Clear styles")
        .def("style", static_cast<void (berialdraw::Styles::*)(const berialdraw::String&)>(&berialdraw::Styles::style),
             py::arg("name"),
             "Set style name");
}
