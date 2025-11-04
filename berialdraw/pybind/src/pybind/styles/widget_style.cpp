#include "pybind/pyberialdraw.hpp"
void bind_widget_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::WidgetStyle, berialdraw::Style>(m, "WidgetStyle")
        .def(pybind11::init<>())
        .def_property("row", 
            static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::row),
            static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::row), "Row position")
        .def_property("column", 
            static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::column),
            static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::column), "Column position")
        .def("cell", &berialdraw::WidgetStyle::cell)
        .def_property("size_policy", 
            static_cast<berialdraw::SizePolicy (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::size_policy),
            static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::SizePolicy)>(&berialdraw::WidgetStyle::size_policy), "Size policy")
        .def_property("min_size", 
            static_cast<const berialdraw::Size& (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::min_size),
            static_cast<void (berialdraw::WidgetStyle::*)(const berialdraw::Size&)>(&berialdraw::WidgetStyle::min_size), 
            "Minimum size", pybind11::return_value_policy::reference_internal)
        .def_property("max_size", 
            static_cast<const berialdraw::Size& (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::max_size),
            static_cast<void (berialdraw::WidgetStyle::*)(const berialdraw::Size&)>(&berialdraw::WidgetStyle::max_size), 
            "Maximum size", pybind11::return_value_policy::reference_internal)
        .def_property("id", 
            static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::id),
            static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::id), "Widget ID")
        .def_property("extend", 
            static_cast<berialdraw::Extend (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::extend),
            static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Extend)>(&berialdraw::WidgetStyle::extend), "Extend mode")
        .def_property("pressed", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressed),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressed), "Pressed state")
        .def_property("pressable", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressable),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressable), "Pressable state")
        .def_property("checked", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::checked),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::checked), "Checked state")
        .def_property("focusable", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focusable),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focusable), "Focusable state")
        .def_property("selectable", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::selectable),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::selectable), "Selectable state")
        .def_property("focused", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focused),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focused), "Focused state");
}
