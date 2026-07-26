#include "pybind/pyberialdraw.hpp"
void bind_padding_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::PaddingStyle, berialdraw::Style> cls(m, "PaddingStyle");
    cls.def(pybind11::init<>());
        
    // padding property avec plusieurs formats CSS - use bind_margin_property
    bind_margin_property(cls, "padding",
        &berialdraw::PaddingStyle::padding,
        static_cast<void (berialdraw::PaddingStyle::*)(berialdraw::Dim)>(&berialdraw::PaddingStyle::padding),
        static_cast<void (berialdraw::PaddingStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::PaddingStyle::padding),
        static_cast<void (berialdraw::PaddingStyle::*)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim)>(&berialdraw::PaddingStyle::padding),
        "Padding: int (all), (v,h), or (top,right,bottom,left)");
}
