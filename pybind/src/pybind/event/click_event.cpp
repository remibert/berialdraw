#include "pybind/pyberialdraw.hpp"
void bind_click_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::ClickEvent, berialdraw::Event>(m, "ClickEvent")
        .def(pybind11::init<const berialdraw::Point&, berialdraw::Widget*>(),
            pybind11::arg("position"), pybind11::arg("clicked") = nullptr)
        .def_property_readonly("position", &berialdraw::ClickEvent::position)
        .def_property_readonly("widget", &berialdraw::ClickEvent::widget, pybind11::return_value_policy::reference);
}