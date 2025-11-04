#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_column(pybind11::module_& m) {
    pybind11::class_<berialdraw::Column, berialdraw::Widget>(m, "Column")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def_property("flow",
            [](berialdraw::Column& self) -> bool { return self.flow(); },
            [](berialdraw::Column& self, bool value) { self.flow(value); }, "Flow state")
        
        // Event system integration (useful for clickable columns)
        BIND_EVENT_PROPERTY(berialdraw::Column, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Column, berialdraw::TouchEvent, on_touch);
}
