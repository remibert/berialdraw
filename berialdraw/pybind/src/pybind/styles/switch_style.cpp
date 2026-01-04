#include "pybind/pyberialdraw.hpp"
void bind_switch_style(py::module& m) {
    py::class_<berialdraw::SwitchStyle, berialdraw::Style> cls(m, "SwitchStyle");
    cls.def(py::init<>(), "Constructor");
        
    // switch_size property - use bind_size_property
    bind_size_property(cls, "switch_size",
        &berialdraw::SwitchStyle::switch_size,
        static_cast<void (berialdraw::SwitchStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::SwitchStyle::switch_size),
        "Switch size: int/float (same w/h) or (width, height) with automatic precision");

    cls.def_property("on_track_color",
        [](berialdraw::SwitchStyle& self) -> uint32_t { return self.on_track_color(); },
        [](berialdraw::SwitchStyle& self, uint32_t value) { self.on_track_color(value); }, "On track color");
    cls.def_property("off_track_color",
        [](berialdraw::SwitchStyle& self) -> uint32_t { return self.off_track_color(); },
        [](berialdraw::SwitchStyle& self, uint32_t value) { self.off_track_color(value); }, "Off track color");
    cls.def_property("thumb_color",
        [](berialdraw::SwitchStyle& self) -> uint32_t { return self.thumb_color(); },
        [](berialdraw::SwitchStyle& self, uint32_t value) { self.thumb_color(value); }, "Thumb color");
    cls.def_property("thumb_padding",
        [](berialdraw::SwitchStyle& self) -> berialdraw::Dim { return self.thumb_padding(); },
        [](berialdraw::SwitchStyle& self, berialdraw::Dim value) { self.thumb_padding(value); }, "Thumb padding");
}
