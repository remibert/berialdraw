#include "pybind/pyberialdraw.hpp"

void bind_scroll_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::ScrollEvent, berialdraw::Event>(m, "ScrollEvent")
        .def(pybind11::init<const berialdraw::Point&, berialdraw::ScrollView*>(), 
             pybind11::arg("shift"), pybind11::arg("scroll_view") = nullptr)
        .def("shift", &berialdraw::ScrollEvent::shift, 
             pybind11::return_value_policy::reference_internal)
        .def_static("type_id", &berialdraw::ScrollEvent::type_id);
}