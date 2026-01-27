#include "pybind/pyberialdraw.hpp"
void bind_checkbox_style(py::module& m) {
    py::class_<berialdraw::CheckboxStyle, berialdraw::Style> cls(m, "CheckboxStyle");
    cls.def(py::init<>(), "Constructor");
        
    // Checkbox box properties
    
    // check_box_size property - use bind_size_property
    bind_size_property(cls, "check_box_size",
        &berialdraw::CheckboxStyle::check_box_size,
        static_cast<void (berialdraw::CheckboxStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::CheckboxStyle::check_box_size),
        "Checkbox box size: int/float (same w/h) or (width, height) with automatic precision");

    cls.def_property("check_box_thickness",
        [](berialdraw::CheckboxStyle& self) -> berialdraw::Dim { return self.check_box_thickness(); },
        [](berialdraw::CheckboxStyle& self, berialdraw::Dim value) { self.check_box_thickness(value); }, 
        "Checkbox box thickness");
    
    cls.def_property("check_box_radius",
        [](berialdraw::CheckboxStyle& self) -> berialdraw::Dim { return self.check_box_radius(); },
        [](berialdraw::CheckboxStyle& self, berialdraw::Dim value) { self.check_box_radius(value); }, 
        "Checkbox box radius");
    
    cls.def_property("check_box_padding",
        [](berialdraw::CheckboxStyle& self) -> berialdraw::Dim { return self.check_box_padding(); },
        [](berialdraw::CheckboxStyle& self, berialdraw::Dim value) { self.check_box_padding(value); }, 
        "Checkbox box padding");

    bind_color_property(cls, "check_box_color",
        &berialdraw::CheckboxStyle::check_box_color,
        static_cast<void (berialdraw::CheckboxStyle::*)(uint32_t)>(&berialdraw::CheckboxStyle::check_box_color),
        "Check box color");
    
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
