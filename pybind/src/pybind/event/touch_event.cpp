#include "pybind/pyberialdraw.hpp"

void bind_touch_event(pybind11::module_& m) {
    // TouchEvent::TouchState enum
    pybind11::enum_<berialdraw::TouchEvent::TouchState>(m, "TouchState")
        .value("TOUCH_UP", berialdraw::TouchEvent::TouchState::TOUCH_UP)
        .value("TOUCH_DOWN", berialdraw::TouchEvent::TouchState::TOUCH_DOWN)
        .value("TOUCH_MOVE", berialdraw::TouchEvent::TouchState::TOUCH_MOVE);

    pybind11::class_<berialdraw::TouchEvent, berialdraw::Event>(m, "TouchEvent")
        .def(pybind11::init<const berialdraw::Point&, berialdraw::TouchEvent::TouchState>(), 
             pybind11::arg("position"), pybind11::arg("state"))
        .def("position", &berialdraw::TouchEvent::position, 
             pybind11::return_value_policy::reference_internal)
        .def("state", &berialdraw::TouchEvent::state)
        .def_static("type_id", &berialdraw::TouchEvent::type_id);
}