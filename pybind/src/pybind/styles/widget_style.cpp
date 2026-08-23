#include "pybind/pyberialdraw.hpp"
void bind_widget_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::WidgetStyle, berialdraw::Style> cls(m, "WidgetStyle");
    cls.def(pybind11::init<>());
    
    cls.def_property(berialdraw::StyleNames::WIDGET_ROW, 
        static_cast<berialdraw::Dim (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::row),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim)>(&berialdraw::WidgetStyle::row), "Row position");
    cls.def_property(berialdraw::StyleNames::WIDGET_COLUMN, 
        static_cast<berialdraw::Dim (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::column),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim)>(&berialdraw::WidgetStyle::column), "Column position");
    cls.def("cell", &berialdraw::WidgetStyle::cell, PYBIND11_RELEASE_GIL);
    cls.def_property(berialdraw::StyleNames::SIZE_POLICY, 
        static_cast<berialdraw::SizePolicy (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::size_policy),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::SizePolicy)>(&berialdraw::WidgetStyle::size_policy), "Size policy");
    
    // min_size and max_size - use bind_size_property
    bind_size_property(cls, berialdraw::StyleNames::WIDGET_MIN_SIZE,
        &berialdraw::WidgetStyle::min_size,
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::WidgetStyle::min_size),
        "Minimum size: int, (w,h), or Size");
    bind_size_property(cls, berialdraw::StyleNames::WIDGET_MAX_SIZE,
        &berialdraw::WidgetStyle::max_size,
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::WidgetStyle::max_size),
        "Maximum size: int, (w,h), or Size");

    cls.def_property(berialdraw::StyleNames::KEY_ID, 
        static_cast<berialdraw::Dim (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::id),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Dim)>(&berialdraw::WidgetStyle::id), "Widget ID");
    cls.def_property(berialdraw::StyleNames::EXTEND, 
        static_cast<berialdraw::Extend (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::extend),
        static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Extend)>(&berialdraw::WidgetStyle::extend), "Extend mode");
    cls.def_property(berialdraw::StyleNames::WIDGET_PRESSED, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressed),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressed), "Pressed state");
    cls.def_property(berialdraw::StyleNames::WIDGET_PRESSABLE, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressable),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressable), "Pressable state");
    cls.def_property(berialdraw::StyleNames::WIDGET_CHECKED, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::checked),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::checked), "Checked state");
    cls.def_property(berialdraw::StyleNames::WIDGET_FOCUSABLE, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focusable),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focusable), "Focusable state");
    cls.def_property(berialdraw::StyleNames::WIDGET_SELECTABLE, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::selectable),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::selectable), "Selectable state");
    cls.def_property(berialdraw::StyleNames::WIDGET_FOCUSED, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focused),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focused), "Focused state");
    cls.def_property(berialdraw::StyleNames::WIDGET_SELECTED, 
        static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::selected),
        static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::selected), "Selected state");
}
