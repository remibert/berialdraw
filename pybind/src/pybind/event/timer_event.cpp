#include "pybind/pyberialdraw.hpp"

void bind_timer_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::TimerEvent, berialdraw::Event>(m, "TimerEvent")
        .def(pybind11::init<uint32_t, uint32_t, berialdraw::Widget*>(),
            pybind11::arg("timer_id"), pybind11::arg("elapsed_ticks") = 1, pybind11::arg("widget") = nullptr)
        .def_property_readonly("timer_id", &berialdraw::TimerEvent::timer_id, "The unique timer identifier")
        .def_property_readonly("elapsed_ticks", &berialdraw::TimerEvent::elapsed_ticks, "Number of elapsed timer periods (1=normal, 2+=catch-up)")
        .def_property_readonly("widget", &berialdraw::TimerEvent::widget, pybind11::return_value_policy::reference, "The Timer widget that triggered this event");
}
