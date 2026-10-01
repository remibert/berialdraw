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
        .def("set", [](berialdraw::Margin& self, double top, double left, double bottom, double right) {
                 self.set_q6(to_q6(top), to_q6(left), to_q6(bottom), to_q6(right));
             },
             py::arg("top"), py::arg("left"), py::arg("bottom"), py::arg("right"),
             "Set margin values with float values (high precision)");
             
    // Propriétés avec précision automatique
    bind_precision_property(cls, "top",
        &berialdraw::Margin::top,
        &berialdraw::Margin::top,
        &berialdraw::Margin::top_q6,
        "Top margin (int for normal, float for high precision)");
    bind_precision_property(cls, "left",
        &berialdraw::Margin::left,
        &berialdraw::Margin::left,
        &berialdraw::Margin::left_q6,
        "Left margin (int for normal, float for high precision)");
    bind_precision_property(cls, "bottom",
        &berialdraw::Margin::bottom,
        &berialdraw::Margin::bottom,
        &berialdraw::Margin::bottom_q6,
        "Bottom margin (int for normal, float for high precision)");
    bind_precision_property(cls, "right",
        &berialdraw::Margin::right,
        &berialdraw::Margin::right,
        &berialdraw::Margin::right_q6,
        "Right margin (int for normal, float for high precision)");
}
