#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_row(pybind11::module_& m) {
    pybind11::class_<berialdraw::Row, berialdraw::Widget>(m, "Row")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def_property("flow",
            [](berialdraw::Row& self) -> bool { return self.flow(); },
            [](berialdraw::Row& self, bool value) { self.flow(value); }, "Flow state")
        
        // Event system integration (useful for clickable rows)
        BIND_EVENT_PROPERTY(berialdraw::Row, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Row, berialdraw::TouchEvent, on_touch);
}
