#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_timer(pybind11::module_& m) {
    pybind11::class_<berialdraw::Timer, berialdraw::Widget, berialdraw::TimerStyle>(m, "Timer")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Timer control methods
        .def("start", &berialdraw::Timer::start, PYBIND11_RELEASE_GIL, "Start the timer")
        .def("stop", &berialdraw::Timer::stop, PYBIND11_RELEASE_GIL, "Stop the timer")
        .def("is_running", &berialdraw::Timer::is_running, PYBIND11_RELEASE_GIL, "Check if timer is running")
        .def("timer_id", &berialdraw::Timer::timer_id, PYBIND11_RELEASE_GIL, "Get the timer ID")
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Timer, berialdraw::TimerEvent, on_timeout);
}
