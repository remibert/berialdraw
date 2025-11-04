#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_progress_bar(pybind11::module_& m) {
    pybind11::class_<berialdraw::ProgressBar, berialdraw::Widget, berialdraw::BorderStyle, berialdraw::ProgressBarStyle>(m, "ProgressBar")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::ProgressBar::*)(const berialdraw::ProgressBar&)>(&berialdraw::ProgressBar::copy))
        
        // Event system integration (useful for interactive progress bars)
        BIND_EVENT_PROPERTY(berialdraw::ProgressBar, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::ProgressBar, berialdraw::TouchEvent, on_touch);
}
