#include "pybind/pyberialdraw.hpp"
void bind_checkbox_style(py::module& m) {
    py::class_<berialdraw::CheckboxStyle, berialdraw::Style> cls(m, "CheckboxStyle");
    cls.def(py::init<>(), "Constructor");
        
    // Checkbox box properties
    
    // checkbox_size property - use bind_size_property
    bind_size_property(cls, "checkbox_size",
        &berialdraw::CheckboxStyle::checkbox_size,
        static_cast<void (berialdraw::CheckboxStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CheckboxStyle::checkbox_size),
        "Checkbox box size: int/float (same w/h) or (width, height) with automatic precision");

    cls.def_property("check_padding",
        [](berialdraw::CheckboxStyle& self) -> berialdraw::Dim { return self.check_padding(); },
        [](berialdraw::CheckboxStyle& self, berialdraw::Dim value) { self.check_padding(value); }, 
        "Checkbox padding in pixels");

    // Check mark properties
    
    bind_color_property(cls, "check_color",
        &berialdraw::CheckboxStyle::check_color,
        static_cast<void (berialdraw::CheckboxStyle::*)(uint32_t)>(&berialdraw::CheckboxStyle::check_color),
        "Check mark color");
    
    cls.def_property("check_sketch",
        [](berialdraw::CheckboxStyle& self) -> std::string { 
            return self.check_sketch().c_str(); 
        },
        [](berialdraw::CheckboxStyle& self, const std::string& value) { 
            self.check_sketch(berialdraw::String(value.c_str())); 
        }, 
        "Check mark sketch (VectorScript)");
}
