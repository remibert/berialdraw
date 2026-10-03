#include "pybind/pyberialdraw.hpp"
void bind_round_style(py::module& m) {
    py::class_<berialdraw::RoundStyle, berialdraw::Style> cls(m, "RoundStyle");
    cls.def(py::init<>(), "Constructor");

    bind_precision_property<berialdraw::RoundStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::BORDER_THICKNESS,
        &berialdraw::RoundStyle::thickness,
        &berialdraw::RoundStyle::thickness,
        &berialdraw::RoundStyle::thickness_q6,
        "Line thickness (int for normal, float for high precision)");
    bind_precision_property<berialdraw::RoundStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::BORDER_RADIUS,
        &berialdraw::RoundStyle::radius,
        &berialdraw::RoundStyle::radius,
        &berialdraw::RoundStyle::radius_q6,
        "Border radius (int for normal, float for high precision)");
}
