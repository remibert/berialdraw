#include "pybind/pyberialdraw.hpp"

void bind_device_screen(py::module& m) {
#if defined(WIN32)
    // Sur Windows, on utilise DeviceWin32 mais on l'expose comme DeviceScreen
    py::class_<berialdraw::DeviceWin32, berialdraw::Device>(m, "DeviceScreen")
        .def(py::init<const char*, berialdraw::Dim, berialdraw::Dim>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0,
             "Constructor")
        .def_static("show_console", &berialdraw::DeviceWin32::show_console,
                   "Show the console");
#else
    // Sur Linux/autres, on utilise DeviceSdl mais on l'expose comme DeviceScreen
    py::class_<berialdraw::DeviceSdl, berialdraw::Device>(m, "DeviceScreen")
        .def(py::init<const char*, berialdraw::Dim, berialdraw::Dim>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0,
             "Constructor");
#endif
}