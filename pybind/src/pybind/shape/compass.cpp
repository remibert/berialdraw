#include "pybind/pyberialdraw.hpp"
void bind_compass(py::module& m) {
    py::class_<berialdraw::Compass>(m, "Compass")
        .def(py::init<>(),
             "Constructor")
        .def("position", &berialdraw::Compass::position,
             "Position given by the compass as a function of the angle and the center")
        
        // Exposer les méthodes utiles de Compass
        .def_property("angle", 
            [](const berialdraw::Compass& self) { return static_cast<const berialdraw::CommonStyle&>(self).angle(); },
            [](berialdraw::Compass& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    static_cast<berialdraw::CommonStyle&>(self).angle(value.cast<int>());
                } else if (py::isinstance<py::float_>(value)) {
                    static_cast<berialdraw::CommonStyle&>(self).angle_(static_cast<int>(value.cast<double>() * 64));
                }
            }, "Angle (int for normal precision, float for high precision)")
            
        .def_property("radius", 
            [](const berialdraw::Compass& self) { return static_cast<const berialdraw::RoundStyle&>(self).radius(); },
            [](berialdraw::Compass& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    static_cast<berialdraw::RoundStyle&>(self).radius(value.cast<int>());
                } else if (py::isinstance<py::float_>(value)) {
                    static_cast<berialdraw::RoundStyle&>(self).radius_(static_cast<int>(value.cast<double>() * 64));
                }
            }, "Radius (int for normal precision, float for high precision)")
            
        .def_property("center", 
            [](const berialdraw::Compass& self) { return static_cast<const berialdraw::CommonStyle&>(self).center(); },
            [](berialdraw::Compass& self, py::object value) {
                if (py::isinstance<berialdraw::Point>(value)) {
                    static_cast<berialdraw::CommonStyle&>(self).center(value.cast<berialdraw::Point>());
                } else if (py::isinstance<py::tuple>(value)) {
                    auto tuple = value.cast<py::tuple>();
                    if (tuple.size() == 2) {
                        if (py::isinstance<py::int_>(tuple[0])) {
                            static_cast<berialdraw::CommonStyle&>(self).center(tuple[0].cast<int>(), tuple[1].cast<int>());
                        } else {
                            static_cast<berialdraw::CommonStyle&>(self).center_(static_cast<int>(tuple[0].cast<double>() * 64), 
                                                                              static_cast<int>(tuple[1].cast<double>() * 64));
                        }
                    }
                }
            }, "Center (Point object, or tuple(x,y) with int/float coordinates)");
}