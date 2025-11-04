#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_label(pybind11::module_& m) {
    pybind11::class_<berialdraw::Label, berialdraw::Widget, berialdraw::TextStyle>(m, "Label")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::Label::*)(const berialdraw::Label&)>(&berialdraw::Label::copy))
        
        // Event system integration (useful for clickable labels)
        BIND_EVENT_PROPERTY(berialdraw::Label, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Label, berialdraw::TouchEvent, on_touch);
}
