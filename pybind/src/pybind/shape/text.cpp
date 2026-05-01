#include "pybind/pyberialdraw.hpp"
void bind_text(py::module& m) {
    py::class_<berialdraw::Text, berialdraw::TextStyle>(m, "Text")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Text&>(),
             py::arg("other"),
             "Copy constructor")
        .def("content_size", &berialdraw::Text::content_size,
             "Get the size of the text content");
}
