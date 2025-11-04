#include "pybind/pyberialdraw.hpp"
void bind_margin(py::module& m) {
    py::class_<berialdraw::Margin>(m, "Margin")
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
             "Set margin values with a precision of 64th of a pixel")
             
        // Propriétés avec précision automatique
        .def_property("top",
            [](berialdraw::Margin& self) -> berialdraw::Dim { return self.top(); },
            [](berialdraw::Margin& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.top(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.top_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("top must be int or float");
                }
            }, "Top margin (int for normal, float for high precision)")
        .def_property("left",
            [](berialdraw::Margin& self) -> berialdraw::Dim { return self.left(); },
            [](berialdraw::Margin& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.left(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.left_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("left must be int or float");
                }
            }, "Left margin (int for normal, float for high precision)")
        .def_property("bottom",
            [](berialdraw::Margin& self) -> berialdraw::Dim { return self.bottom(); },
            [](berialdraw::Margin& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.bottom(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.bottom_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("bottom must be int or float");
                }
            }, "Bottom margin (int for normal, float for high precision)")
        .def_property("right",
            [](berialdraw::Margin& self) -> berialdraw::Dim { return self.right(); },
            [](berialdraw::Margin& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.right(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.right_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("right must be int or float");
                }
            }, "Right margin (int for normal, float for high precision)");
}
