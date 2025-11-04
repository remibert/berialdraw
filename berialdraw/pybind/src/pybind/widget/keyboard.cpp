#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_keyboard(pybind11::module_& m) {
    pybind11::class_<berialdraw::Keyboard, berialdraw::Widget, berialdraw::TextStyle, berialdraw::BorderStyle>(m, "Keyboard")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::Keyboard::*)(const berialdraw::Keyboard&)>(&berialdraw::Keyboard::copy))
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Keyboard, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Keyboard, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Keyboard, berialdraw::TouchEvent, on_touch);
}
