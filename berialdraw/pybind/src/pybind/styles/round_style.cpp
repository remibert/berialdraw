#include "pybind/pyberialdraw.hpp"
void bind_round_style(py::module& m) {
    py::class_<berialdraw::RoundStyle, berialdraw::Style>(m, "RoundStyle")
        .def(py::init<>(), "Constructor")
        
        // Propriétés avec précision automatique int/float
        .def_property("thickness",
            [](berialdraw::RoundStyle& self) -> berialdraw::Dim { return self.thickness(); },
            [](berialdraw::RoundStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.thickness(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.thickness_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("thickness must be int or float");
                }
            }, "Line thickness (int for normal, float for high precision)")
        .def_property("radius",
            [](berialdraw::RoundStyle& self) -> berialdraw::Dim { return self.radius(); },
            [](berialdraw::RoundStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.radius(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.radius_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("radius must be int or float");
                }
            }, "Border radius (int for normal, float for high precision)");
}
