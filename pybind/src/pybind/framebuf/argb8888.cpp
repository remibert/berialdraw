#include "pybind/pyberialdraw.hpp"
void bind_argb8888(py::module& m) {
    // Argb8888 - Usage interne pour l'initialisation seulement
    py::class_<berialdraw::Argb8888, berialdraw::Framebuf>(m, "Argb8888")
        .def(py::init<uint32_t, uint32_t>(),
             py::arg("width"), py::arg("height"),
             "Constructor");
}
