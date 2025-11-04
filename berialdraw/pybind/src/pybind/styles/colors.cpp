#include "pybind/pyberialdraw.hpp"

void bind_colors(py::module& m) {
    py::class_<berialdraw::Colors>(m, "Colors")
        .def("appearance", &berialdraw::Colors::appearance,
             py::arg("name") = nullptr,
             "Choose the name of the appearance mode (can be 'light', 'dark')")
        .def("theme", &berialdraw::Colors::theme,
             py::arg("color"),
             "Choose the color theme")
        .def("color", static_cast<void (berialdraw::Colors::*)(uint32_t, uint32_t, bool)>(&berialdraw::Colors::color),
             py::arg("id"), py::arg("color"), py::arg("focused") = false,
             "Redefine predefined color")
        .def("color", static_cast<uint32_t (berialdraw::Colors::*)(uint32_t, bool)>(&berialdraw::Colors::color),
             py::arg("id"), py::arg("focused") = false,
             "Return the color according to its id");
}
