#include "pybind/pyberialdraw.hpp"
void bind_border_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::BorderStyle, berialdraw::Style> cls(m, "BorderStyle");
    cls.def(pybind11::init<>());
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, "radius",
        &berialdraw::BorderStyle::radius,
        &berialdraw::BorderStyle::radius,
        "Border radius");
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, "thickness",
        &berialdraw::BorderStyle::thickness,
        &berialdraw::BorderStyle::thickness,
        "Border thickness");
    
    bind_color_property(cls, "border_color",
        &berialdraw::BorderStyle::border_color,
        static_cast<void (berialdraw::BorderStyle::*)(uint32_t)>(&berialdraw::BorderStyle::border_color),
        "Border color");
    bind_color_property(cls, "focus_color",
        &berialdraw::BorderStyle::focus_color,
        static_cast<void (berialdraw::BorderStyle::*)(uint32_t)>(&berialdraw::BorderStyle::focus_color),
        "Focus color");
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, "focus_gap",
        &berialdraw::BorderStyle::focus_gap,
        &berialdraw::BorderStyle::focus_gap,
        "Focus gap");
    
    bind_scalar_property<berialdraw::BorderStyle, berialdraw::Dim>(cls, "focus_thickness",
        &berialdraw::BorderStyle::focus_thickness,
        &berialdraw::BorderStyle::focus_thickness,
        "Focus thickness");
}
