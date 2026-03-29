#include "pybind/pyberialdraw.hpp"

// Template function to bind device screen implementations
template<typename DeviceImpl>
void bind_device_screen_impl(py::module& m) {
    py::class_<DeviceImpl, berialdraw::Device> cls(m, "DeviceScreen");
    cls.def(py::init<const char*, berialdraw::Dim, berialdraw::Dim, berialdraw::Coord, berialdraw::Coord>(),
             py::arg("title"), py::arg("width") = 0, py::arg("height") = 0, py::arg("x") = 0, py::arg("y") = 0,
             "Constructor with position");
    
    // size property using custom wrapper
    cls.def_property("size",
        [](DeviceImpl& self) -> py::tuple {
            const auto& s = self.size();
            return py::make_tuple(s.width(), s.height());
        },
        [](DeviceImpl& self, py::object value) {
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
        [](DeviceImpl& self) -> py::tuple {
            const auto& p = self.position();
            return py::make_tuple(p.x(), p.y());
        },
        [](DeviceImpl& self, py::object value) {
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

	cls.def_static("show_console", &DeviceImpl::show_console,
				   "Show the console");
}

void bind_device_screen(py::module& m) {
#if defined(WIN32)
    bind_device_screen_impl<berialdraw::DeviceWin32>(m);
#elif defined(OSX) && defined(USE_COCOA_DEVICE)
    bind_device_screen_impl<berialdraw::DeviceCocoa>(m);
#else
    bind_device_screen_impl<berialdraw::DeviceSdl>(m);
#endif
}