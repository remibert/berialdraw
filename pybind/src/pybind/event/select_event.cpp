#include "pybind/pyberialdraw.hpp"

void bind_select_event(pybind11::module_& m) {
    // SelectEvent::Status enum
    pybind11::enum_<berialdraw::SelectEvent::Status>(m, "SelectStatus")
        .value("SELECT_START", berialdraw::SelectEvent::Status::SELECT_START)
        .value("SELECT_PROGRESS", berialdraw::SelectEvent::Status::SELECT_PROGRESS)
        .value("SELECT_END", berialdraw::SelectEvent::Status::SELECT_END);

    pybind11::class_<berialdraw::SelectEvent, berialdraw::Event>(m, "SelectEvent")
        .def(pybind11::init<berialdraw::SelectEvent::Status, const berialdraw::Point&, berialdraw::Widget*>(), 
             pybind11::arg("status"), pybind11::arg("position"), pybind11::arg("widget") = nullptr)
        .def("status", &berialdraw::SelectEvent::status)
        .def("position", &berialdraw::SelectEvent::position, 
             pybind11::return_value_policy::reference_internal)
        .def_static("type_id", &berialdraw::SelectEvent::type_id);
}