#include "pybind/pyberialdraw.hpp"
void bind_popup(pybind11::module_& m) {
    pybind11::class_<berialdraw::Popup, berialdraw::Pane, std::unique_ptr<berialdraw::Popup, pybind11::nodelete>>(m, "Popup")
        .def(pybind11::init<berialdraw::Window*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("open", &berialdraw::Popup::open, pybind11::keep_alive<1, 2>())
        .def("open_at", &berialdraw::Popup::open_at)
        .def("close", &berialdraw::Popup::close)
        .def_property_readonly("is_open", &berialdraw::Popup::is_open);
}