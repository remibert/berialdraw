#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_window(pybind11::module_& m) {
    pybind11::class_<berialdraw::Window, berialdraw::Widget>(m, "Window")
        .def(pybind11::init<>())
        .def("back", &berialdraw::Window::back)
        .def("front", &berialdraw::Window::front)
        .def("copy", static_cast<void (berialdraw::Window::*)(const berialdraw::Window&)>(&berialdraw::Window::copy))
        
        // Event system integration (useful for background interactions)
        BIND_EVENT_PROPERTY(berialdraw::Window, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Window, berialdraw::TouchEvent, on_touch);
}
