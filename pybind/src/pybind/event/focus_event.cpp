#include "pybind/pyberialdraw.hpp"

void bind_focus_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::FocusEvent, berialdraw::Event>(m, "FocusEvent")
        .def(pybind11::init<bool, berialdraw::Widget*>(), 
             pybind11::arg("focused"), pybind11::arg("widget") = nullptr)
        .def("focused", &berialdraw::FocusEvent::focused)
        .def_static("type_id", &berialdraw::FocusEvent::type_id);
}