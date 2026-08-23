#include "pybind/pyberialdraw.hpp"
void bind_slider_style(py::module& m) {
    py::class_<berialdraw::SliderStyle, berialdraw::Style> cls(m, "SliderStyle");
    cls.def(py::init<>(), "Constructor");
    
    bind_color_property(cls, berialdraw::StyleNames::SLIDER_TRACK_COLOR,
        &berialdraw::SliderStyle::track_color,
        static_cast<void (berialdraw::SliderStyle::*)(uint32_t)>(&berialdraw::SliderStyle::track_color),
        "Track color");
    bind_color_property(cls, berialdraw::StyleNames::SLIDER_HANDLE_COLOR,
        &berialdraw::SliderStyle::handle_color,
        static_cast<void (berialdraw::SliderStyle::*)(uint32_t)>(&berialdraw::SliderStyle::handle_color),
        "Handle color");
    
    cls.def_property(berialdraw::StyleNames::SLIDER_HANDLE_SIZE,
            [](berialdraw::SliderStyle& self) -> berialdraw::Dim { return self.handle_size(); },
            [](berialdraw::SliderStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.handle_size(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.handle_size_q6(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("handle_size must be int or float");
                }
            }, "Handle size (int for normal, float for high precision)")
        .def_property(berialdraw::StyleNames::SLIDER_TRACK_SIZE,
            [](berialdraw::SliderStyle& self) -> berialdraw::Dim { return self.track_size(); },
            [](berialdraw::SliderStyle& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.track_size(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.track_size_q6(static_cast<berialdraw::Coord>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("track_size must be int or float");
                }
            }, "Track size (int for normal, float for high precision)")
        .def_property(berialdraw::StyleNames::RANGE_VALUE,
            [](berialdraw::SliderStyle& self) -> int32_t { return self.value(); },
            [](berialdraw::SliderStyle& self, int32_t value) { self.value(value); }, "Current value")
        .def_property(berialdraw::StyleNames::RANGE_MIN_VALUE,
            [](berialdraw::SliderStyle& self) -> int32_t { return self.min_value(); },
            [](berialdraw::SliderStyle& self, int32_t value) { self.min_value(value); }, "Minimum value")
        .def_property(berialdraw::StyleNames::RANGE_MAX_VALUE,
            [](berialdraw::SliderStyle& self) -> int32_t { return self.max_value(); },
            [](berialdraw::SliderStyle& self, int32_t value) { self.max_value(value); }, "Maximum value")
        .def_property(berialdraw::StyleNames::RANGE_STEP_VALUE,
            [](berialdraw::SliderStyle& self) -> uint32_t { return self.step_value(); },
            [](berialdraw::SliderStyle& self, uint32_t value) { self.step_value(value); }, "Step value");
}
