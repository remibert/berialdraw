#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_switch(pybind11::module_& m) {
    pybind11::class_<berialdraw::Switch, berialdraw::Widget, berialdraw::BorderStyle, berialdraw::SwitchStyle>(m, "Switch")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Switch, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Switch, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Switch, berialdraw::CheckEvent, on_check)
        BIND_EVENT_PROPERTY(berialdraw::Switch, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Switch, berialdraw::TouchEvent, on_touch);
}
