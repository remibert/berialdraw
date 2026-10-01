#include "pybind/pyberialdraw.hpp"

void bind_scrollbar_style(py::module& m) {
    py::class_<berialdraw::ScrollbarStyle, berialdraw::Style> cls(m, "ScrollbarStyle");
    cls.def(py::init<>(), "Constructor")
        .def_property(berialdraw::StyleNames::SCROLLBAR_VISIBLE,
            [](berialdraw::ScrollbarStyle& self) -> bool { 
                return self.scrollbar_visible(); 
            },
            [](berialdraw::ScrollbarStyle& self, bool v) { 
                self.scrollbar_visible(v); 
            }, "Whether scrollbar is visible")
        .def_property(berialdraw::StyleNames::SCROLLBAR_THUMB_COLOR,
            [](berialdraw::ScrollbarStyle& self) -> uint32_t { 
                return self.scrollbar_thumb_color(); 
            },
            [](berialdraw::ScrollbarStyle& self, uint32_t c) { 
                self.scrollbar_thumb_color(c); 
            }, "Scrollbar thumb color");

    bind_precision_property<berialdraw::ScrollbarStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::SCROLLBAR_WIDTH,
        &berialdraw::ScrollbarStyle::scrollbar_width,
        &berialdraw::ScrollbarStyle::scrollbar_width,
        &berialdraw::ScrollbarStyle::scrollbar_width_q6,
        "Scrollbar width (int for normal, float for high precision)");
    bind_precision_property<berialdraw::ScrollbarStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::SCROLLBAR_RADIUS,
        &berialdraw::ScrollbarStyle::scrollbar_radius,
        &berialdraw::ScrollbarStyle::scrollbar_radius,
        &berialdraw::ScrollbarStyle::scrollbar_radius_q6,
        "Scrollbar corner radius (int for normal, float for high precision)");
    bind_precision_property<berialdraw::ScrollbarStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::SCROLLBAR_MARGIN,
        &berialdraw::ScrollbarStyle::scrollbar_margin,
        &berialdraw::ScrollbarStyle::scrollbar_margin,
        &berialdraw::ScrollbarStyle::scrollbar_margin_q6,
        "Scrollbar margin (int for normal, float for high precision)");
}
