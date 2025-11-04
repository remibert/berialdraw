#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_slider(pybind11::module_& m) {
    pybind11::class_<berialdraw::Slider, berialdraw::Widget, berialdraw::BorderStyle, berialdraw::SliderStyle>(m, "Slider")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::Slider::*)(const berialdraw::Slider&)>(&berialdraw::Slider::copy))
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Slider, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Slider, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Slider, berialdraw::SlideEvent, on_slide)
        BIND_EVENT_PROPERTY(berialdraw::Slider, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Slider, berialdraw::TouchEvent, on_touch);
}
