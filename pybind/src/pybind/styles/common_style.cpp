#include "pybind/pyberialdraw.hpp"
void bind_common_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::CommonStyle, berialdraw::Style> cls(m, "CommonStyle");

    cls.def(pybind11::init<>());

    // Use generic margin binder with pointer-to-member setters
    bind_margin_property(cls, berialdraw::StyleNames::COMMON_MARGIN,
        &berialdraw::CommonStyle::margin,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim)>(&berialdraw::CommonStyle::margin),
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CommonStyle::margin),
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CommonStyle::margin),
        &berialdraw::CommonStyle::margin_q6,
        "Margin: int/float (all), (horizontal, vertical), or (top,left,bottom,right); float for high precision");

    // point properties: center and position
    bind_point_property(cls, berialdraw::StyleNames::COMMON_CENTER,
        &berialdraw::CommonStyle::center,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::CommonStyle::center),
        &berialdraw::CommonStyle::center_q6,
        "Center as (x, y) tuple, float for high precision");

    bind_point_property(cls, berialdraw::StyleNames::COMMON_POSITION,
        &berialdraw::CommonStyle::position,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::CommonStyle::position),
        &berialdraw::CommonStyle::position_q6,
        "Position as (x, y) tuple, float for high precision");

    // size property using generic pair binder
    bind_size_property(cls, berialdraw::StyleNames::COMMON_SIZE,
        &berialdraw::CommonStyle::size,
        static_cast<void (berialdraw::CommonStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CommonStyle::size),
        &berialdraw::CommonStyle::size_q6,
        "Size: int/float (square) or (width, height); float for high precision");

    // The remaining simple properties
    bind_color_property(cls, berialdraw::StyleNames::COMMON_COLOR,
        &berialdraw::CommonStyle::color,
        static_cast<void (berialdraw::CommonStyle::*)(uint32_t)>(&berialdraw::CommonStyle::color),
        static_cast<void (berialdraw::CommonStyle::*)(uint32_t, uint8_t)>(&berialdraw::CommonStyle::color),
        "Color (accepts both uint32_t and Color enum)");
    bind_precision_property<berialdraw::CommonStyle, berialdraw::Coord>(cls, berialdraw::StyleNames::COMMON_ANGLE,
        &berialdraw::CommonStyle::angle,
        &berialdraw::CommonStyle::angle,
        &berialdraw::CommonStyle::angle_q6,
        "Rotation angle (int for normal, float for high precision)");
    cls.def_property(berialdraw::StyleNames::COMMON_ALIGN,
        [](berialdraw::CommonStyle& self) -> berialdraw::Align { return self.align(); },
        [](berialdraw::CommonStyle& self, berialdraw::Align value) { self.align(value); }, "Alignment");
    cls.def_property(berialdraw::StyleNames::WIDGET_BORDERS,
        [](berialdraw::CommonStyle& self) -> uint16_t { return self.borders(); },
        [](berialdraw::CommonStyle& self, uint16_t value) { self.borders(value); }, "Borders");
    cls.def_property(berialdraw::StyleNames::COMMON_HIDDEN,
        [](berialdraw::CommonStyle& self) -> bool { return self.hidden(); },
        [](berialdraw::CommonStyle& self, bool value) { self.hidden(value); }, "Hidden state");
}
