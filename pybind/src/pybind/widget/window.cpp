#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_window(pybind11::module_& m) {
    pybind11::class_<berialdraw::Window, berialdraw::Widget>(m, "Window")
        .def(pybind11::init<>())
        .def("back", &berialdraw::Window::back, PYBIND11_RELEASE_GIL)
        .def("front", &berialdraw::Window::front, PYBIND11_RELEASE_GIL)
        
        // Event system integration (useful for background interactions)
        BIND_EVENT_PROPERTY(berialdraw::Window, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Window, berialdraw::TouchEvent, on_touch);
}
