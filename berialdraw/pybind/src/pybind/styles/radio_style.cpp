#include "pybind/pyberialdraw.hpp"
void bind_radio_style(py::module& m) {
    py::class_<berialdraw::RadioStyle, berialdraw::Style> cls(m, "RadioStyle");
    cls.def(py::init<>(), "Constructor");
        
    // Radio box properties
    
    // radio_size property - use bind_size_property
    bind_size_property(cls, "radio_size",
        &berialdraw::RadioStyle::radio_size,
        static_cast<void (berialdraw::RadioStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::RadioStyle::radio_size),
        "Radio box size: int/float (same w/h) or (width, height) with automatic precision");

    cls.def_property("radio_padding",
        [](berialdraw::RadioStyle& self) -> berialdraw::Dim { return self.radio_padding(); },
        [](berialdraw::RadioStyle& self, berialdraw::Dim value) { self.radio_padding(value); }, 
        "Radio padding in pixels");

    // Radio indicator properties
    
    bind_color_property(cls, "radio_color",
        &berialdraw::RadioStyle::radio_color,
        static_cast<void (berialdraw::RadioStyle::*)(uint32_t)>(&berialdraw::RadioStyle::radio_color),
        "Radio indicator color");
    
    cls.def_property("radio_sketch",
        [](berialdraw::RadioStyle& self) -> std::string { 
            return self.radio_sketch().c_str(); 
        },
        [](berialdraw::RadioStyle& self, const std::string& value) { 
            self.radio_sketch(berialdraw::String(value.c_str())); 
        }, 
        "Radio indicator sketch (VectorScript)");
    
    cls.def_property("group",
        [](berialdraw::RadioStyle& self) -> std::string { 
            return self.group().c_str(); 
        },
        [](berialdraw::RadioStyle& self, const std::string& value) { 
            self.group(berialdraw::String(value.c_str())); 
        }, 
        "Radio group name for mutually exclusive selection");
}
