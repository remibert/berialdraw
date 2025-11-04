#include "pybind/pyberialdraw.hpp"

void bind_slide_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::SlideEvent, berialdraw::Event>(m, "SlideEvent")
        .def(pybind11::init<int32_t, berialdraw::Widget*>(), 
             pybind11::arg("value"), pybind11::arg("widget") = nullptr)
        .def("value", &berialdraw::SlideEvent::value)
        .def_static("type_id", &berialdraw::SlideEvent::type_id);
}