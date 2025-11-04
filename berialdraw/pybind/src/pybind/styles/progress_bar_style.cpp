#include "pybind/pyberialdraw.hpp"
void bind_progress_bar_style(py::module& m) {
    py::class_<berialdraw::ProgressBarStyle, berialdraw::Style>(m, "ProgressBarStyle")
        .def(py::init<>(), "Constructor")
        
        // Propriétés Python avec précision automatique
        .def_property("track_color",
            [](berialdraw::ProgressBarStyle& self) -> uint32_t { return self.track_color(); },
            [](berialdraw::ProgressBarStyle& self, uint32_t value) { self.track_color(value); }, "Track color")
        .def_property("fill_color",
            [](berialdraw::ProgressBarStyle& self) -> uint32_t { return self.fill_color(); },
            [](berialdraw::ProgressBarStyle& self, uint32_t value) { self.fill_color(value); }, "Fill color")
        .def_property("fill_size",
            [](berialdraw::ProgressBarStyle& self) -> berialdraw::Dim { return self.fill_size(); },
            [](berialdraw::ProgressBarStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.fill_size(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.fill_size_(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("fill_size must be int or float");
                }
            }, "Fill size (int for normal, float for high precision)")
        .def_property("track_size",
            [](berialdraw::ProgressBarStyle& self) -> berialdraw::Dim { return self.track_size(); },
            [](berialdraw::ProgressBarStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.track_size(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.track_size_(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("track_size must be int or float");
                }
            }, "Track size (int for normal, float for high precision)")
        .def_property("value",
            [](berialdraw::ProgressBarStyle& self) -> int32_t { return self.value(); },
            [](berialdraw::ProgressBarStyle& self, int32_t value) { self.value(value); }, "Current value")
        .def_property("min_value",
            [](berialdraw::ProgressBarStyle& self) -> int32_t { return self.min_value(); },
            [](berialdraw::ProgressBarStyle& self, int32_t value) { self.min_value(value); }, "Minimum value")
        .def_property("max_value",
            [](berialdraw::ProgressBarStyle& self) -> int32_t { return self.max_value(); },
            [](berialdraw::ProgressBarStyle& self, int32_t value) { self.max_value(value); }, "Maximum value")
        .def_property("step_value",
            [](berialdraw::ProgressBarStyle& self) -> uint32_t { return self.step_value(); },
            [](berialdraw::ProgressBarStyle& self, uint32_t value) { self.step_value(value); }, "Step value");
}
