#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_radio(pybind11::module_& m) {
    pybind11::class_<berialdraw::Radio, berialdraw::Widget, berialdraw::TextStyle, berialdraw::BorderStyle, berialdraw::RadioStyle>(m, "Radio")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::TouchEvent, on_touch);
}
