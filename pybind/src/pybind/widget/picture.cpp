#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_picture(pybind11::module_& m) {
    pybind11::class_<berialdraw::Picture, berialdraw::Widget, berialdraw::BorderStyle, berialdraw::PictureStyle>(m, "Picture")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())

        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Picture, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Picture, berialdraw::TouchEvent, on_touch);
}
