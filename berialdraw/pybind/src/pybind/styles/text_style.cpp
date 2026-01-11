#include "pybind/pyberialdraw.hpp"
void bind_text_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::TextStyle, berialdraw::Style> cls(m, "TextStyle");
    cls.def(pybind11::init<>());
        
    // font_familly property 
    cls.def_property("font_familly",
        [](berialdraw::TextStyle& self) -> std::string { return std::string(self.font_familly().c_str()); },
        [](berialdraw::TextStyle& self, const std::string& value) { self.font_familly(value.c_str()); }, "Font family name");
    
    // font_size property avec plusieurs formats - use bind_size_property
    bind_size_property(cls, "font_size",
        &berialdraw::TextStyle::font_size,
        static_cast<void (berialdraw::TextStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::TextStyle::font_size),
        "Font size: int (same w/h) or (width, height)");
    
    // padding property avec plusieurs formats CSS - use bind_margin_property
    bind_margin_property(cls, "padding",
        &berialdraw::TextStyle::padding,
        static_cast<void (berialdraw::TextStyle::*)(berialdraw::Dim)>(&berialdraw::TextStyle::padding),
        static_cast<void (berialdraw::TextStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::TextStyle::padding),
        static_cast<void (berialdraw::TextStyle::*)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim)>(&berialdraw::TextStyle::padding),
        "Padding: int (all), (v,h), or (top,right,bottom,left)");

    cls.def_property("text",
        [](berialdraw::TextStyle& self) -> std::string { return std::string(self.text().c_str()); },
        [](berialdraw::TextStyle& self, const std::string& value) { self.text(value.c_str()); }, "Text content");
    bind_color_property(cls, "text_color",
        &berialdraw::TextStyle::text_color,
        static_cast<void (berialdraw::TextStyle::*)(uint32_t)>(&berialdraw::TextStyle::text_color),
        "Text color");
    cls.def_property("text_align",
        [](berialdraw::TextStyle& self) -> berialdraw::Align { return self.text_align(); },
        [](berialdraw::TextStyle& self, berialdraw::Align value) { self.text_align(value); }, "Text alignment");
}
