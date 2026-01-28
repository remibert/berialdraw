#include "pybind/pyberialdraw.hpp"

void bind_device_screen(py::module& m) {
#if defined(WIN32)
    #define DEVICE berialdraw::DeviceWin32
#elif defined(OSX) && defined(USE_COCOA_DEVICE)
    #define DEVICE berialdraw::DeviceCocoa
#else
    #define DEVICE berialdraw::DeviceSdl
#endif

    py::class_<DEVICE, berialdraw::Device> cls(m, "DeviceScreen");
    cls.def(py::init<const char*, berialdraw::Dim, berialdraw::Dim, berialdraw::Coord, berialdraw::Coord>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0, py::arg("x") = 0, py::arg("y") = 0,
             "Constructor with position");
    // size property using custom wrapper
    cls.def_property("size",
        [](DEVICE& self) -> py::tuple {
            const auto& s = self.size();
            return py::make_tuple(s.width(), s.height());
        },
        [](DEVICE& self, py::object value) {
            if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto dim = value.cast<berialdraw::Dim>();
                self.size(berialdraw::Size(dim, dim));
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.size(berialdraw::Size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>()));
                } else {
                    throw std::invalid_argument("size must be tuple/list of 2 values (width, height)");
                }
            } else {
                throw std::invalid_argument("size must be int/float or tuple/list of 2 values");
            }
        }, "Size: int (square) or (width, height)");
    // position property using custom wrapper
    cls.def_property("position",
        [](DEVICE& self) -> py::tuple {
            const auto& p = self.position();
            return py::make_tuple(p.x(), p.y());
        },
        [](DEVICE& self, py::object value) {
            if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.position(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                } else {
                    throw std::invalid_argument("position must be tuple/list of 2 values (x, y)");
                }
            } else {
                throw std::invalid_argument("position must be tuple/list of 2 values");
            }
        }, "Position as (x, y) tuple");

#if defined(WIN32)
    cls.def_static("show_console", &DEVICE::show_console,
                   "Show the console");
#endif

#undef DEVICE
}