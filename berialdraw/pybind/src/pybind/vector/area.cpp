#include "pybind/pyberialdraw.hpp"
void bind_area(py::module& m) {
    py::class_<berialdraw::Area> cls(m, "Area");

    cls.def(py::init<const berialdraw::Point&, const berialdraw::Size&>(),
             py::arg("position"), py::arg("size"),
             "Create area with position and size")
        .def(py::init<berialdraw::Coord, berialdraw::Coord, berialdraw::Dim, berialdraw::Dim, bool>(),
             py::arg("x") = 0, py::arg("y") = 0, py::arg("w") = 0, py::arg("h") = 0, py::arg("pixel") = true,
             "Create area with coordinates")
        .def(py::init<const berialdraw::Area&>(),
             py::arg("other"),
             "Create a copy of area")
        .def("move", &berialdraw::Area::move,
             py::arg("shift"),
             "Move the point by specified coordinates with a precision of 64th of a pixel")
        .def("set", &berialdraw::Area::set,
             py::arg("position"), py::arg("size"),
             "Set the position and size of the area")
        .def("nearest_pixel", &berialdraw::Area::nearest_pixel,
             "Resizes and move itself on the nearest pixel")
        .def("decrease", &berialdraw::Area::decrease,
             py::arg("margin"),
             "Decrease size and position of area with margin")
        .def("increase", &berialdraw::Area::increase,
             py::arg("margin"),
             "Increase size and position of area with margin")
        .def("print", &berialdraw::Area::print,
             py::arg("name"),
             "Print content");

    // Propriétés avec précision automatique
    cls.def_property("width",
        [](berialdraw::Area& self) -> berialdraw::Dim { return self.width(); },
        [](berialdraw::Area& self, py::object value) {
            if (py::isinstance<py::int_>(value)) {
                self.width(value.cast<berialdraw::Dim>());
            } else if (py::isinstance<py::float_>(value)) {
                self.width_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
            } else {
                throw std::invalid_argument("width must be int or float");
            }
        }, "Width (int for normal, float for high precision)");
    cls.def_property("height",
        [](berialdraw::Area& self) -> berialdraw::Dim { return self.height(); },
        [](berialdraw::Area& self, py::object value) {
            if (py::isinstance<py::int_>(value)) {
                self.height(value.cast<berialdraw::Dim>());
            } else if (py::isinstance<py::float_>(value)) {
                self.height_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
            } else {
                throw std::invalid_argument("height must be int or float");
            }
        }, "Height (int for normal, float for high precision)");
    cls.def_property("x",
        [](berialdraw::Area& self) -> berialdraw::Coord { return self.x(); },
        [](berialdraw::Area& self, py::object value) {
            if (py::isinstance<py::int_>(value)) {
                self.x(value.cast<berialdraw::Coord>());
            } else if (py::isinstance<py::float_>(value)) {
                self.x_(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
            } else {
                throw std::invalid_argument("x must be int or float");
            }
        }, "X coordinate (int for normal, float for high precision)");
    cls.def_property("y",
        [](berialdraw::Area& self) -> berialdraw::Coord { return self.y(); },
        [](berialdraw::Area& self, py::object value) {
            if (py::isinstance<py::int_>(value)) {
                self.y(value.cast<berialdraw::Coord>());
            } else if (py::isinstance<py::float_>(value)) {
                self.y_(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
            } else {
                throw std::invalid_argument("y must be int or float");
            }
        }, "Y coordinate (int for normal, float for high precision)");

    // position and size use generic binders adapted for Area
    cls.def_property("position",
        [](berialdraw::Area& self) -> py::tuple {
            const auto& p = self.position();
            return py::make_tuple(p.x(), p.y());
        },
        [](berialdraw::Area& self, py::object value) {
            if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.position(berialdraw::Point(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>()));
                } else {
                    throw std::invalid_argument("position must be tuple/list of 2 values (x, y)");
                }
            } else {
                throw std::invalid_argument("position must be tuple/list of 2 values");
            }
        }, "Position as (x, y) tuple");

    cls.def_property("size",
        [](berialdraw::Area& self) -> py::tuple {
            const auto& s = self.size();
            return py::make_tuple(s.width(), s.height());
        },
        [](berialdraw::Area& self, py::object value) {
            if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto dim = value.cast<berialdraw::Dim>();
                self.size(berialdraw::Size(dim, dim));
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.size(berialdraw::Size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>()));
                } else {
                    throw std::invalid_argument("size must be tuple/list of 2 values (width, height)");
                }
            } else {
                throw std::invalid_argument("size must be int/float or tuple/list of 2 values");
            }
        }, "Size as (width, height) tuple");
}
