#include "pybind/pyberialdraw.hpp"
void bind_grid(pybind11::module_& m) {
    pybind11::class_<berialdraw::Grid, berialdraw::Widget>(m, "Grid")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>());
}
