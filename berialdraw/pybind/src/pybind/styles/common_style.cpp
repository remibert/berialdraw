#include "pybind/pyberialdraw.hpp"
void bind_common_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::CommonStyle, berialdraw::Style> cls(m, "CommonStyle");

    cls.def(pybind11::init<>());

    // Use generic margin binder with pointer-to-member setters
    bind_margin_property(cls, "margin",
        &berialdraw::CommonStyle::margin,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim)>(&berialdraw::CommonStyle::margin),
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CommonStyle::margin),
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CommonStyle::margin),
        "Margin: int (all), (v,h), or (top,right,bottom,left)");

    // point properties: center and position
    bind_point_property(cls, "center",
        &berialdraw::CommonStyle::center,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::CommonStyle::center),
        "Center as (x, y) tuple");

    bind_point_property(cls, "position",
        &berialdraw::CommonStyle::position,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::CommonStyle::position),
        "Position as (x, y) tuple");

    // size property using generic pair binder
    bind_size_property(cls, "size",
        &berialdraw::CommonStyle::size,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CommonStyle::size),
        "Size: int (square) or (width, height)");

    // The remaining simple properties
    bind_color_property(cls, "color",
        &berialdraw::CommonStyle::color,
        static_cast<void (berialdraw::CommonStyle::*)(uint32_t)>(&berialdraw::CommonStyle::color),
        "Color (accepts both uint32_t and Color enum)");
    cls.def_property("angle",
        [](berialdraw::CommonStyle& self) -> berialdraw::Coord { return self.angle(); },
        [](berialdraw::CommonStyle& self, berialdraw::Coord value) { self.angle(value); }, "Rotation angle");
    cls.def_property("align",
        [](berialdraw::CommonStyle& self) -> berialdraw::Align { return self.align(); },
        [](berialdraw::CommonStyle& self, berialdraw::Align value) { self.align(value); }, "Alignment");
    cls.def_property("borders",
        [](berialdraw::CommonStyle& self) -> uint16_t { return self.borders(); },
        [](berialdraw::CommonStyle& self, uint16_t value) { self.borders(value); }, "Borders");
    cls.def_property("hidden",
        [](berialdraw::CommonStyle& self) -> bool { return self.hidden(); },
        [](berialdraw::CommonStyle& self, bool value) { self.hidden(value); }, "Hidden state");
}
