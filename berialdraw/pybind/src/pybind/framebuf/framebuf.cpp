#include "pybind/pyberialdraw.hpp"
void bind_framebuf(py::module& m) {
    auto framebuf_class = py::class_<berialdraw::Framebuf>(m, "Framebuf")
        .def_static("create", &berialdraw::Framebuf::create, 
                   py::arg("width"), py::arg("height"), py::arg("type"),
                   "Create a framebuffer",
                   py::return_value_policy::reference_internal);

    py::enum_<berialdraw::Framebuf::Type>(framebuf_class, "Type")
        .value("ARGB8888", berialdraw::Framebuf::Type::ARGB8888)
        .export_values();
}
