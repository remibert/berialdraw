#include "pybind/pyberialdraw.hpp"
void bind_icon_style(py::module& m) {
    py::class_<berialdraw::IconStyle, berialdraw::Style>(m, "IconStyle")
        .def(py::init<>(), "Constructor")
        
        // Propriétés avec précision automatique
        .def_property("zoom",
            [](berialdraw::IconStyle& self) -> berialdraw::Dim { return self.zoom(); },
            [](berialdraw::IconStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.zoom(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.zoom_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("zoom must be int or float");
                }
            }, "Zoom ratio (int for normal, float for high precision)")
        .def_property("icon_padding",
            [](berialdraw::IconStyle& self) -> py::tuple {
                const auto& margin = self.icon_padding();
                return py::make_tuple(margin.top(), margin.right(), margin.bottom(), margin.left());
            },
            [](berialdraw::IconStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                    // icon_padding = 5 -> (5, 5, 5, 5)
                    auto dim = value.cast<double>();
                    if (py::isinstance<py::int_>(value)) {
                        self.icon_padding(static_cast<berialdraw::Dim>(dim), static_cast<berialdraw::Dim>(dim), 
                                        static_cast<berialdraw::Dim>(dim), static_cast<berialdraw::Dim>(dim));
                    } else {
                        self.icon_padding_(static_cast<berialdraw::Dim>(dim * 64), static_cast<berialdraw::Dim>(dim * 64), 
                                         static_cast<berialdraw::Dim>(dim * 64), static_cast<berialdraw::Dim>(dim * 64));
                    }
                } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    auto len = py::len(seq);
                    if (len == 4) {
                        // icon_padding = (1, 2, 3, 4) -> (top, right, bottom, left)
                        if (py::isinstance<py::int_>(seq[0]) && py::isinstance<py::int_>(seq[1]) && 
                            py::isinstance<py::int_>(seq[2]) && py::isinstance<py::int_>(seq[3])) {
                            self.icon_padding(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>(), 
                                            seq[2].cast<berialdraw::Dim>(), seq[3].cast<berialdraw::Dim>());
                        } else {
                            auto top = seq[0].cast<double>();
                            auto right = seq[1].cast<double>();
                            auto bottom = seq[2].cast<double>();
                            auto left = seq[3].cast<double>();
                            self.icon_padding_(static_cast<berialdraw::Dim>(top * 64), static_cast<berialdraw::Dim>(right * 64), 
                                             static_cast<berialdraw::Dim>(bottom * 64), static_cast<berialdraw::Dim>(left * 64));
                        }
                    } else {
                        throw std::invalid_argument("icon_padding tuple/list must have 4 values");
                    }
                } else {
                    throw std::invalid_argument("icon_padding must be int/float or tuple/list of 4 values");
                }
            }, "Icon padding: int/float (all) or (top,right,bottom,left) with automatic precision")
        .def_property("filename",
            [](berialdraw::IconStyle& self) -> std::string { return std::string(self.filename().c_str()); },
            [](berialdraw::IconStyle& self, const std::string& value) { self.filename(value.c_str()); }, "Icon filename")
        .def_property("icon_color",
            [](berialdraw::IconStyle& self) -> uint32_t { return self.icon_color(); },
            [](berialdraw::IconStyle& self, uint32_t value) { self.icon_color(value); }, "Icon color");
}
