#include "pybind/pyberialdraw.hpp"
void bind_style(py::module& m) {
    py::class_<berialdraw::Style>(m, "Style");
}
