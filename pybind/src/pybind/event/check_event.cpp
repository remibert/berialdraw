#include "pybind/pyberialdraw.hpp"

void bind_check_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::CheckEvent, berialdraw::Event>(m, "CheckEvent")
        .def(pybind11::init<bool, berialdraw::Widget*>(), 
             pybind11::arg("checked"), pybind11::arg("widget") = nullptr)
        .def("checked", &berialdraw::CheckEvent::checked)
        .def_static("type_id", &berialdraw::CheckEvent::type_id);
}