#include "pybind/pyberialdraw.hpp"

void bind_timer_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::TimerStyle, berialdraw::Style> cls(m, "TimerStyle");
    cls.def(pybind11::init<>(), "Constructor");

    // interval property (in milliseconds)
    cls.def_property("interval",
        [](berialdraw::TimerStyle& self) -> int {
            return (int)self.interval();
        },
        [](berialdraw::TimerStyle& self, int value) {
            if (value < 0) value = 0;
            self.interval((uint32_t)value);
        },
        "Timer interval in milliseconds");

    // recurring property
    cls.def_property("recurring",
        [](berialdraw::TimerStyle& self) -> bool {
            return self.recurring();
        },
        [](berialdraw::TimerStyle& self, bool value) {
            self.recurring(value);
        },
        "Whether timer repeats (True) or fires once (False)");

    // active property
    cls.def_property("active",
        [](berialdraw::TimerStyle& self) -> bool {
            return self.active();
        },
        [](berialdraw::TimerStyle& self, bool value) {
            self.active(value);
        },
        "Whether timer is running");
}
