#include "pybind/pyberialdraw.hpp"

void bind_styles(py::module& m) {
    py::class_<berialdraw::Styles>(m, "Styles")
        .def(py::init<>(),
             "Create styles")
        .def("clear", &berialdraw::Styles::clear,
             "Clear all styles")
        .def("style", py::overload_cast<>(&berialdraw::Styles::style),
             "Get current style name")
        .def("style", py::overload_cast<const berialdraw::String&>(&berialdraw::Styles::style),
             py::arg("name"),
             "Set current style name")
        .def("add_style", static_cast<void (berialdraw::Styles::*)(const berialdraw::String&, const berialdraw::String&)>(&berialdraw::Styles::add_style),
             py::arg("name"), py::arg("properties"),
             "Add a style with name and JSON properties")
        .def("remove_style", &berialdraw::Styles::remove_style,
             py::arg("name"),
             "Remove a style by name")
        .def("get_style", &berialdraw::Styles::get_style,
             py::arg("name"),
             "Get a style by name")
        .def("has_style", &berialdraw::Styles::has_style,
             py::arg("name"),
             "Check if a style exists by name")
        .def("mappings", &berialdraw::Styles::mappings,
             py::arg("classname"),
             "Get mappings properties for a class")
        .def("keys", &berialdraw::Styles::keys,
             py::arg("classname"),
             "Get keys properties for a class");
}
