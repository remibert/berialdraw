#include "pybind/pyberialdraw.hpp"
void bind_margin(py::module& m) {
    auto cls = py::class_<berialdraw::Margin>(m, "Margin")
        .def(py::init<int>(),
             py::arg("v"),
             "Constructor with a single value for all margins")
        .def(py::init<berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, bool>(),
             py::arg("t") = 0, py::arg("l") = 0, py::arg("b") = 0, py::arg("r") = 0, py::arg("pixel") = true,
             "Constructor with individual margin values")
        .def(py::init<const berialdraw::Margin&>(),
             py::arg("p"),
             "Copy constructor")
        .def("__eq__", &berialdraw::Margin::operator==,
             py::arg("other"),
             "Equality operator")
        .def("__ne__", &berialdraw::Margin::operator!=,
             py::arg("other"),
             "Inequality operator")
        .def("set", &berialdraw::Margin::set,
             py::arg("top"), py::arg("left"), py::arg("bottom"), py::arg("right"),
             "Set margin values")
        .def("set_", &berialdraw::Margin::set_,
             py::arg("top"), py::arg("left"), py::arg("bottom"), py::arg("right"),
             "Set margin values with a precision of 64th of a pixel");
             
    // Propriétés avec précision automatique
    bind_precision_property(cls, "top",
        &berialdraw::Margin::top,
        &berialdraw::Margin::top,
        &berialdraw::Margin::top_,
        "Top margin (int for normal, float for high precision)");
    bind_precision_property(cls, "left",
        &berialdraw::Margin::left,
        &berialdraw::Margin::left,
        &berialdraw::Margin::left_,
        "Left margin (int for normal, float for high precision)");
    bind_precision_property(cls, "bottom",
        &berialdraw::Margin::bottom,
        &berialdraw::Margin::bottom,
        &berialdraw::Margin::bottom_,
        "Bottom margin (int for normal, float for high precision)");
    bind_precision_property(cls, "right",
        &berialdraw::Margin::right,
        &berialdraw::Margin::right,
        &berialdraw::Margin::right_,
        "Right margin (int for normal, float for high precision)");
}
