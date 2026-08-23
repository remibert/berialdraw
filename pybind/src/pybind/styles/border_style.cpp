#include "pybind/pyberialdraw.hpp"
void bind_border_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::BorderStyle, berialdraw::Style> cls(m, "BorderStyle");
    cls.def(pybind11::init<>());
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::BORDER_RADIUS,
        &berialdraw::BorderStyle::radius,
        &berialdraw::BorderStyle::radius,
        "Border radius");
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::BORDER_THICKNESS,
        &berialdraw::BorderStyle::thickness,
        &berialdraw::BorderStyle::thickness,
        "Border thickness");
    
    bind_color_property(cls, berialdraw::StyleNames::BORDER_COLOR,
        &berialdraw::BorderStyle::border_color,
        static_cast<void (berialdraw::BorderStyle::*)(uint32_t)>(&berialdraw::BorderStyle::border_color),
        "Border color");
    bind_color_property(cls, berialdraw::StyleNames::BORDER_FOCUS_COLOR,
        &berialdraw::BorderStyle::focus_color,
        static_cast<void (berialdraw::BorderStyle::*)(uint32_t)>(&berialdraw::BorderStyle::focus_color),
        "Focus color");
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::BORDER_FOCUS_GAP,
        &berialdraw::BorderStyle::focus_gap,
        &berialdraw::BorderStyle::focus_gap,
        "Focus gap");
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, berialdraw::StyleNames::BORDER_FOCUS_THICKNESS,
        &berialdraw::BorderStyle::focus_thickness,
        &berialdraw::BorderStyle::focus_thickness,
        "Focus thickness");
}
