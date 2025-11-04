#include "pybind/pyberialdraw.hpp"
void bind_line_style(py::module& m) {
    py::class_<berialdraw::LineStyle, berialdraw::Style>(m, "LineStyle")
        .def(py::init<>(), "Constructor")
        
        // Propriétés avec précision automatique et support tuple
        .def_property("point1",
            [](berialdraw::LineStyle& self) -> py::tuple {
                const auto& point = self.point1();
                return py::make_tuple(point.x(), point.y());
            },
            [](berialdraw::LineStyle& self, py::object value) {
                if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        // point1 = (x, y) avec détection int/float
                        if (py::isinstance<py::int_>(seq[0]) && py::isinstance<py::int_>(seq[1])) {
                            // Cas entier: point1(x, y)
                            self.point1(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                        } else {
                            // Cas float: point1_(x*64, y*64)
                            auto x = seq[0].cast<double>();
                            auto y = seq[1].cast<double>();
                            self.point1_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));
                        }
                    } else {
                        throw std::invalid_argument("point1 tuple/list must have 2 values (x, y)");
                    }
                } else {
                    throw std::invalid_argument("point1 must be tuple/list of 2 values");
                }
            }, "Point1 as (x, y) tuple with automatic precision")
        .def_property("point2",
            [](berialdraw::LineStyle& self) -> py::tuple {
                const auto& point = self.point2();
                return py::make_tuple(point.x(), point.y());
            },
            [](berialdraw::LineStyle& self, py::object value) {
                if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        // point2 = (x, y) avec détection int/float
                        if (py::isinstance<py::int_>(seq[0]) && py::isinstance<py::int_>(seq[1])) {
                            // Cas entier: point2(x, y)
                            self.point2(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                        } else {
                            // Cas float: point2_(x*64, y*64)
                            auto x = seq[0].cast<double>();
                            auto y = seq[1].cast<double>();
                            self.point2_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));
                        }
                    } else {
                        throw std::invalid_argument("point2 tuple/list must have 2 values (x, y)");
                    }
                } else {
                    throw std::invalid_argument("point2 must be tuple/list of 2 values");
                }
            }, "Point2 as (x, y) tuple with automatic precision");
}
