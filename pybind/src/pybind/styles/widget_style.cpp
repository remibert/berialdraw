#include "pybind/pyberialdraw.hpp"
void bind_widget_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::WidgetStyle, berialdraw::Style> cls(m, "WidgetStyle");
    cls.def(pybind11::init<>());
    
    cls.def_property("row", 
        static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::row),
        static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::row), "Row position");
    cls.def_property("column", 
        static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::column),
        static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::column), "Column position");
    cls.def("cell", &berialdraw::WidgetStyle::cell);
    cls.def_property("size_policy", 
        static_cast<berialdraw::SizePolicy (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::size_policy),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::SizePolicy)>(&berialdraw::WidgetStyle::size_policy), "Size policy");
    
    // min_size and max_size - use bind_size_property
    bind_size_property(cls, "min_size",
        &berialdraw::WidgetStyle::min_size,
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::WidgetStyle::min_size),
        "Minimum size: int, (w,h), or Size");
    bind_size_property(cls, "max_size",
        &berialdraw::WidgetStyle::max_size,
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::WidgetStyle::max_size),
        "Maximum size: int, (w,h), or Size");

    cls.def_property("id", 
        static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::id),
        static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::id), "Widget ID");
    cls.def_property("extend", 
        static_cast<berialdraw::Extend (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::extend),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Extend)>(&berialdraw::WidgetStyle::extend), "Extend mode");
    cls.def_property("pressed", 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressed),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressed), "Pressed state");
    cls.def_property("pressable", 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressable),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressable), "Pressable state");
    cls.def_property("checked", 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::checked),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::checked), "Checked state");
    cls.def_property("focusable", 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focusable),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focusable), "Focusable state");
    cls.def_property("selectable", 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::selectable),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::selectable), "Selectable state");
    cls.def_property("focused", 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focused),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focused), "Focused state");
}
