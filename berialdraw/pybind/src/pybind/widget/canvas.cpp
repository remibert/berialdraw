#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_canvas(pybind11::module_& m) {
    pybind11::class_<berialdraw::Canvas, berialdraw::Widget>(m, "Canvas")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("add", &berialdraw::Canvas::add)
        .def("remove", &berialdraw::Canvas::remove)
        .def("clear", &berialdraw::Canvas::clear)
        
        // Event system integration (useful for interactive graphics)
        BIND_EVENT_PROPERTY(berialdraw::Canvas, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Canvas, berialdraw::TouchEvent, on_touch);
}
