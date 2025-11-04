#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_edit(pybind11::module_& m) {
    // Edit inherits from Widget, TextStyle, BorderStyle, EditStyle, and Entry
    pybind11::class_<berialdraw::Edit, berialdraw::Widget, berialdraw::TextStyle, berialdraw::BorderStyle, berialdraw::EditStyle, berialdraw::Entry>(m, "Edit")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::Edit::*)(const berialdraw::Edit&)>(&berialdraw::Edit::copy))
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Edit, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Edit, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Edit, berialdraw::SelectEvent, on_select)
        BIND_EVENT_PROPERTY(berialdraw::Edit, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Edit, berialdraw::TouchEvent, on_touch);
}
