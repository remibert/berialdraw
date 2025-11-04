#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_button(pybind11::module_& m) {
    // Note: Les CallbackManager sont bindés une seule fois dans un module séparé
    
    pybind11::class_<berialdraw::Button, berialdraw::Widget, berialdraw::TextStyle, berialdraw::BorderStyle>(m, "Button")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::Button::*)(const berialdraw::Button&)>(&berialdraw::Button::copy))
        
        // Event system integration using generic system
        BIND_EVENT_PROPERTY(berialdraw::Button, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Button, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Button, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Button, berialdraw::TouchEvent, on_touch);
}
