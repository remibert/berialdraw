#include "pybind/pyberialdraw.hpp"
void bind_pane(pybind11::module_& m) {
    pybind11::class_<berialdraw::Pane, berialdraw::Widget>(m, "Pane")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        .def("copy", static_cast<void (berialdraw::Pane::*)(const berialdraw::Pane&)>(&berialdraw::Pane::copy));
}
