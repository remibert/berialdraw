#include "pybind/pyberialdraw.hpp"
void bind_pie_style(py::module& m) {
    py::class_<berialdraw::PieStyle, berialdraw::Style>(m, "PieStyle")
        .def(py::init<>(), "Constructor")
        
        // Propriétés avec précision automatique int/float
        .def_property("start_angle",
            [](berialdraw::PieStyle& self) -> berialdraw::Coord { return self.start_angle(); },
            [](berialdraw::PieStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.start_angle(value.cast<berialdraw::Coord>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.start_angle_(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("start_angle must be int or float");
                }
            }, "Start angle (int for normal, float for high precision)")
        .def_property("end_angle",
            [](berialdraw::PieStyle& self) -> berialdraw::Coord { return self.end_angle(); },
            [](berialdraw::PieStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.end_angle(value.cast<berialdraw::Coord>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.end_angle_(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("end_angle must be int or float");
                }
            }, "End angle (int for normal, float for high precision)")
        .def_property("rope",
            [](berialdraw::PieStyle& self) -> bool { return self.rope(); },
            [](berialdraw::PieStyle& self, bool value) { self.rope(value); }, "Rope mode");
}
