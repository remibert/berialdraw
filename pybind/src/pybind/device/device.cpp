#include "pybind/pyberialdraw.hpp"
void bind_device(py::module& m) {
    py::class_<berialdraw::Device>(m, "Device");
}
