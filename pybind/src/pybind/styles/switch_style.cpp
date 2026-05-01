#include "pybind/pyberialdraw.hpp"
void bind_switch_style(py::module& m) {
    py::class_<berialdraw::SwitchStyle, berialdraw::Style> cls(m, "SwitchStyle");
    cls.def(py::init<>(), "Constructor");
        
    // switch_size property - use bind_size_property
    bind_size_property(cls, "switch_size",
        &berialdraw::SwitchStyle::switch_size,
        static_cast<void (berialdraw::SwitchStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::SwitchStyle::switch_size),
        "Switch size: int/float (same w/h) or (width, height) with automatic precision");

    bind_color_property(cls, "on_track_color",
        &berialdraw::SwitchStyle::on_track_color,
        static_cast<void (berialdraw::SwitchStyle::*)(uint32_t)>(&berialdraw::SwitchStyle::on_track_color),
        "On track color");
    bind_color_property(cls, "off_track_color",
        &berialdraw::SwitchStyle::off_track_color,
        static_cast<void (berialdraw::SwitchStyle::*)(uint32_t)>(&berialdraw::SwitchStyle::off_track_color),
        "Off track color");
    bind_color_property(cls, "thumb_color",
        &berialdraw::SwitchStyle::thumb_color,
        static_cast<void (berialdraw::SwitchStyle::*)(uint32_t)>(&berialdraw::SwitchStyle::thumb_color),
        "Thumb color");
    cls.def_property("thumb_padding",
        [](berialdraw::SwitchStyle& self) -> berialdraw::Dim { return self.thumb_padding(); },
        [](berialdraw::SwitchStyle& self, berialdraw::Dim value) { self.thumb_padding(value); }, "Thumb padding");
}
