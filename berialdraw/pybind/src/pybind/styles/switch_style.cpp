#include "pybind/pyberialdraw.hpp"
void bind_switch_style(py::module& m) {
    py::class_<berialdraw::SwitchStyle, berialdraw::Style>(m, "SwitchStyle")
        .def(py::init<>(), "Constructor")
        
        // switch_size property avec gestion automatique int/float et précision
        .def_property("switch_size",
            [](berialdraw::SwitchStyle& self) -> py::tuple {
                const auto& size = self.switch_size();
                return py::make_tuple(size.width(), size.height());
            },
            [](berialdraw::SwitchStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                    // switch_size = 12 -> (12, 12)
                    if (py::isinstance<py::int_>(value)) {
                        auto dim = value.cast<berialdraw::Dim>();
                        self.switch_size(dim, dim);
                    } else {
                        auto dim_float = value.cast<double>();
                        self.switch_size_(static_cast<berialdraw::Coord>(dim_float * 64), 
                                        static_cast<berialdraw::Coord>(dim_float * 64));
                    }
                } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        // switch_size = (12, 16) ou (12.5, 16.3)
                        if (py::isinstance<py::int_>(seq[0]) && py::isinstance<py::int_>(seq[1])) {
                            // Cas entier: switch_size(w, h)
                            self.switch_size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                        } else {
                            // Cas float: switch_size_(w*64, h*64)
                            auto w = seq[0].cast<double>();
                            auto h = seq[1].cast<double>();
                            self.switch_size_(static_cast<berialdraw::Coord>(w * 64), 
                                            static_cast<berialdraw::Coord>(h * 64));
                        }
                    } else {
                        throw std::invalid_argument("switch_size tuple/list must have 2 values (width, height)");
                    }
                } else {
                    throw std::invalid_argument("switch_size must be int/float or tuple/list of 2 values");
                }
            }, "Switch size: int/float (same w/h) or (width, height) with automatic precision")

         .def_property("on_track_color",
            [](berialdraw::SwitchStyle& self) -> uint32_t { return self.on_track_color(); },
            [](berialdraw::SwitchStyle& self, uint32_t value) { self.on_track_color(value); }, "On track color")
        .def_property("off_track_color",
            [](berialdraw::SwitchStyle& self) -> uint32_t { return self.off_track_color(); },
            [](berialdraw::SwitchStyle& self, uint32_t value) { self.off_track_color(value); }, "Off track color")
        .def_property("thumb_color",
            [](berialdraw::SwitchStyle& self) -> uint32_t { return self.thumb_color(); },
            [](berialdraw::SwitchStyle& self, uint32_t value) { self.thumb_color(value); }, "Thumb color")
        .def_property("thumb_padding",
            [](berialdraw::SwitchStyle& self) -> berialdraw::Dim { return self.thumb_padding(); },
            [](berialdraw::SwitchStyle& self, berialdraw::Dim value) { self.thumb_padding(value); }, "Thumb padding");
}
