#include "pybind/pyberialdraw.hpp"
void bind_common_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::CommonStyle, berialdraw::Style>(m, "CommonStyle")
        .def(pybind11::init<>())
        
        // Propriétés avec formats flexibles
        .def_property("margin",
            [](berialdraw::CommonStyle& self) -> py::tuple {
                const auto& margin = self.margin();
                return py::make_tuple(margin.top(), margin.right(), margin.bottom(), margin.left());
            },
            [](berialdraw::CommonStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                    // margin = 5 -> (5, 5, 5, 5)
                    auto dim = value.cast<berialdraw::Dim>();
                    self.margin(dim);
                } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    auto len = py::len(seq);
                    if (len == 2) {
                        // margin = (5, 10) -> (5, 10, 5, 10) - vertical, horizontal
                        auto vertical = seq[0].cast<berialdraw::Dim>();
                        auto horizontal = seq[1].cast<berialdraw::Dim>();
                        self.margin(vertical, horizontal);
                    } else if (len == 4) {
                        // margin = (1, 2, 3, 4) -> (top, right, bottom, left)
                        self.margin(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>(), 
                                  seq[2].cast<berialdraw::Dim>(), seq[3].cast<berialdraw::Dim>());
                    } else {
                        throw std::invalid_argument("margin tuple/list must have 2 or 4 values");
                    }
                } else {
                    throw std::invalid_argument("margin must be int/float or tuple/list");
                }
            }, "Margin: int (all), (v,h), or (top,right,bottom,left)")
        .def_property("center",
            [](berialdraw::CommonStyle& self) -> py::tuple {
                const auto& point = self.center();
                return py::make_tuple(point.x(), point.y());
            },
            [](berialdraw::CommonStyle& self, py::object value) {
                if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        self.center(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                    } else {
                        throw std::invalid_argument("center must be tuple/list of 2 values (x, y)");
                    }
                } else {
                    throw std::invalid_argument("center must be tuple/list of 2 values");
                }
            }, "Center as (x, y) tuple")
        .def_property("position",
            [](berialdraw::CommonStyle& self) -> py::tuple {
                const auto& point = self.position();
                return py::make_tuple(point.x(), point.y());
            },
            [](berialdraw::CommonStyle& self, py::object value) {
                if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        self.position(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                    } else {
                        throw std::invalid_argument("position must be tuple/list of 2 values (x, y)");
                    }
                } else {
                    throw std::invalid_argument("position must be tuple/list of 2 values");
                }
            }, "Position as (x, y) tuple")
        .def_property("color",
            [](berialdraw::CommonStyle& self) -> uint32_t { return self.color(); },
            [](berialdraw::CommonStyle& self, uint32_t value) { self.color(value); }, "Color")
        .def_property("size",
            [](berialdraw::CommonStyle& self) -> py::tuple {
                const auto& size = self.size();
                return py::make_tuple(size.width(), size.height());
            },
            [](berialdraw::CommonStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                    // size = 100 -> (100, 100)
                    auto dim = value.cast<berialdraw::Dim>();
                    self.size(dim, dim);
                } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        // size = (100, 50) -> (width=100, height=50)
                        self.size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                    } else {
                        throw std::invalid_argument("size tuple/list must have 2 values (width, height)");
                    }
                } else {
                    throw std::invalid_argument("size must be int/float or tuple/list of 2 values");
                }
            }, "Size: int (square) or (width, height)")
        
        // Propriétés simples
        .def_property("angle",
            [](berialdraw::CommonStyle& self) -> berialdraw::Coord { return self.angle(); },
            [](berialdraw::CommonStyle& self, berialdraw::Coord value) { self.angle(value); }, "Rotation angle")
        .def_property("align",
            [](berialdraw::CommonStyle& self) -> berialdraw::Align { return self.align(); },
            [](berialdraw::CommonStyle& self, berialdraw::Align value) { self.align(value); }, "Alignment")
        .def_property("borders",
            [](berialdraw::CommonStyle& self) -> uint16_t { return self.borders(); },
            [](berialdraw::CommonStyle& self, uint16_t value) { self.borders(value); }, "Borders")
        .def_property("hidden",
            [](berialdraw::CommonStyle& self) -> bool { return self.hidden(); },
            [](berialdraw::CommonStyle& self, bool value) { self.hidden(value); }, "Hidden state");
}
