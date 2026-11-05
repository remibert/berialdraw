#include "pybind/pyberialdraw.hpp"

void bind_combo_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::ComboStyle, berialdraw::Style>(m, "ComboStyle")
        .def(pybind11::init<>())
        .def_property("editable",
            static_cast<bool (berialdraw::ComboStyle::*)() const>(&berialdraw::ComboStyle::editable),
            static_cast<void (berialdraw::ComboStyle::*)(bool)>(&berialdraw::ComboStyle::editable))
        .def_property("button_width",
            static_cast<uint16_t (berialdraw::ComboStyle::*)() const>(&berialdraw::ComboStyle::button_width),
            static_cast<void (berialdraw::ComboStyle::*)(uint16_t)>(&berialdraw::ComboStyle::button_width))
        .def_property("max_popup_height",
            static_cast<uint16_t (berialdraw::ComboStyle::*)() const>(&berialdraw::ComboStyle::max_popup_height),
            static_cast<void (berialdraw::ComboStyle::*)(uint16_t)>(&berialdraw::ComboStyle::max_popup_height));
}