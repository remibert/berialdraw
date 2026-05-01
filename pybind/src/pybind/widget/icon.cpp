#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_icon(pybind11::module_& m) {
    pybind11::class_<berialdraw::Icon, berialdraw::Widget, berialdraw::BorderStyle, berialdraw::IconStyle, berialdraw::TextStyle>(m, "Icon")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Event system integration (Icon is described as a specialized button)
        BIND_EVENT_PROPERTY(berialdraw::Icon, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Icon, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Icon, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Icon, berialdraw::TouchEvent, on_touch);
}
