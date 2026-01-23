#include "pybind/pyberialdraw.hpp"

void bind_device_screen(py::module& m) {
#if defined(WIN32)
    // Sur Windows, on utilise DeviceWin32 mais on l'expose comme DeviceScreen
    py::class_<berialdraw::DeviceWin32, berialdraw::Device>(m, "DeviceScreen")
        .def(py::init<const char*, berialdraw::Dim, berialdraw::Dim, berialdraw::Coord, berialdraw::Coord>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0, py::arg("x") = 0, py::arg("y") = 0,
             "Constructor with position")
        .def("size", (berialdraw::Size (berialdraw::DeviceWin32::*)() const) &berialdraw::DeviceWin32::size,
             "Get the size of the window")
        .def("size", (void (berialdraw::DeviceWin32::*)(const berialdraw::Size&)) &berialdraw::DeviceWin32::size,
             py::arg("s"), "Set the size of the window")
        .def("size", (void (berialdraw::DeviceWin32::*)(berialdraw::Dim, berialdraw::Dim)) &berialdraw::DeviceWin32::size,
             py::arg("width"), py::arg("height"), "Resize the window")
        .def("position", (berialdraw::Point (berialdraw::DeviceWin32::*)() const) &berialdraw::DeviceWin32::position,
             "Get the position of the window")
        .def("position", (void (berialdraw::DeviceWin32::*)(const berialdraw::Point&)) &berialdraw::DeviceWin32::position,
             py::arg("p"), "Set the position of the window")
        .def("position", (void (berialdraw::DeviceWin32::*)(berialdraw::Coord, berialdraw::Coord)) &berialdraw::DeviceWin32::position,
             py::arg("x"), py::arg("y"), "Move the window")
        .def_static("show_console", &berialdraw::DeviceWin32::show_console,
                   "Show the console");
#elif defined(OSX) && defined(USE_COCOA_DEVICE)
    // Sur macOS avec Cocoa, on utilise DeviceCocoa mais on l'expose comme DeviceScreen
    py::class_<berialdraw::DeviceCocoa, berialdraw::Device>(m, "DeviceScreen")
        .def(py::init<const char*, berialdraw::Dim, berialdraw::Dim, berialdraw::Coord, berialdraw::Coord>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0, py::arg("x") = 0, py::arg("y") = 0,
             "Constructor with position")
        .def("size", (berialdraw::Size (berialdraw::DeviceCocoa::*)() const) &berialdraw::DeviceCocoa::size,
             "Get the size of the window")
        .def("size", (void (berialdraw::DeviceCocoa::*)(const berialdraw::Size&)) &berialdraw::DeviceCocoa::size,
             py::arg("s"), "Set the size of the window")
        .def("size", (void (berialdraw::DeviceCocoa::*)(berialdraw::Dim, berialdraw::Dim)) &berialdraw::DeviceCocoa::size,
             py::arg("width"), py::arg("height"), "Resize the window")
        .def("position", (berialdraw::Point (berialdraw::DeviceCocoa::*)() const) &berialdraw::DeviceCocoa::position,
             "Get the position of the window")
        .def("position", (void (berialdraw::DeviceCocoa::*)(const berialdraw::Point&)) &berialdraw::DeviceCocoa::position,
             py::arg("p"), "Set the position of the window")
        .def("position", (void (berialdraw::DeviceCocoa::*)(berialdraw::Coord, berialdraw::Coord)) &berialdraw::DeviceCocoa::position,
             py::arg("x"), py::arg("y"), "Move the window");
#else
    // Sur Linux/autres, on utilise DeviceSdl mais on l'expose comme DeviceScreen
    py::class_<berialdraw::DeviceSdl, berialdraw::Device>(m, "DeviceScreen")
        .def(py::init<const char*, berialdraw::Dim, berialdraw::Dim, berialdraw::Coord, berialdraw::Coord>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0, py::arg("x") = 0, py::arg("y") = 0,
             "Constructor with position")
        .def("size", (berialdraw::Size (berialdraw::DeviceSdl::*)() const) &berialdraw::DeviceSdl::size,
             "Get the size of the window")
        .def("size", (void (berialdraw::DeviceSdl::*)(const berialdraw::Size&)) &berialdraw::DeviceSdl::size,
             py::arg("s"), "Set the size of the window")
        .def("size", (void (berialdraw::DeviceSdl::*)(berialdraw::Dim, berialdraw::Dim)) &berialdraw::DeviceSdl::size,
             py::arg("width"), py::arg("height"), "Resize the window")
        .def("position", (berialdraw::Point (berialdraw::DeviceSdl::*)() const) &berialdraw::DeviceSdl::position,
             "Get the position of the window")
        .def("position", (void (berialdraw::DeviceSdl::*)(const berialdraw::Point&)) &berialdraw::DeviceSdl::position,
             py::arg("p"), "Set the position of the window")
        .def("position", (void (berialdraw::DeviceSdl::*)(berialdraw::Coord, berialdraw::Coord)) &berialdraw::DeviceSdl::position,
             py::arg("x"), py::arg("y"), "Move the window");
#endif
}