#include "pybind/pyberialdraw.hpp"
void bind_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::Event>(m, "Event")
        .def_property_readonly("type", &berialdraw::Event::type)
        .def_property_readonly("widget", &berialdraw::Event::widget, pybind11::return_value_policy::reference)
        .def("to_string", [](berialdraw::Event& self) {
            berialdraw::String str;
            self.to_string(str);
            return str;
        });
}