#include "pybind/pyberialdraw.hpp"
void bind_size(py::module& m) {
    py::class_<berialdraw::Size>(m, "Size")
        .def(py::init<>(), "Create a size")
        .def(py::init<berialdraw::Dim, berialdraw::Dim, bool>(),
             py::arg("w"), py::arg("h"), py::arg("pixel") = true,
             "Create a size with width and height")
        .def(py::init<const berialdraw::Size&>(),
             py::arg("p"),
             "Create a copy of a size")
        .def("__eq__", &berialdraw::Size::operator==,
             py::arg("other"),
             "Check if the size is equal")
        .def("__ne__", &berialdraw::Size::operator!=,
             py::arg("other"),
             "Check if the size is not equal")
        .def("set", &berialdraw::Size::set,
             py::arg("w"), py::arg("h"),
             "Set size with width and height in pixels")
        .def("set_", &berialdraw::Size::set_,
             py::arg("w"), py::arg("h"),
             "Set size with a precision of 64th of a pixel")
        .def("middle", &berialdraw::Size::middle,
             "Get the middle of size")
        .def("decrease", py::overload_cast<const berialdraw::Margin&>(&berialdraw::Size::decrease),
             py::arg("margin"),
             "Decrease size with margin")
        .def("increase", py::overload_cast<const berialdraw::Margin&>(&berialdraw::Size::increase),
             py::arg("margin"),
             "Increase size with margin")
        .def("decrease", py::overload_cast<const berialdraw::Size&>(&berialdraw::Size::decrease),
             py::arg("size"),
             "Decrease size with size")
        .def("increase", py::overload_cast<const berialdraw::Size&>(&berialdraw::Size::increase),
             py::arg("size"),
             "Increase size with size")
        .def("decrease", py::overload_cast<berialdraw::Dim, berialdraw::Dim>(&berialdraw::Size::decrease),
             py::arg("w"), py::arg("h"),
             "Decrease size with width and height in pixels")
        .def("increase", py::overload_cast<berialdraw::Dim, berialdraw::Dim>(&berialdraw::Size::increase),
             py::arg("w"), py::arg("h"),
             "Increase size with width and height in pixels")
        .def("decrease_", &berialdraw::Size::decrease_,
             py::arg("w"), py::arg("h"),
             "Decrease size with a precision of 64th of a pixel")
        .def("increase_", &berialdraw::Size::increase_,
             py::arg("w"), py::arg("h"),
             "Increase size with a precision of 64th of a pixel")
        .def("nearest_pixel", &berialdraw::Size::nearest_pixel,
             "Resizes itself on the nearest pixel")
        .def("is_width_undefined", &berialdraw::Size::is_width_undefined,
             "Indicates if width is not defined")
        .def("is_height_undefined", &berialdraw::Size::is_height_undefined,
             "Indicates if height is not defined")
        .def("clean", &berialdraw::Size::clean,
             "Clean the size and set to undefined")
        .def("print", &berialdraw::Size::print,
             py::arg("name"),
             "Print content")
             
        // Propriétés avec précision automatique
        .def_property("width",
            [](berialdraw::Size& self) -> berialdraw::Dim { return self.width(); },
            [](berialdraw::Size& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.width(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.width_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("width must be int or float");
                }
            }, "Width (int for normal, float for high precision)")
        .def_property("height",
            [](berialdraw::Size& self) -> berialdraw::Dim { return self.height(); },
            [](berialdraw::Size& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.height(value.cast<berialdraw::Dim>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.height_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
                } else {
                    throw std::invalid_argument("height must be int or float");
                }
            }, "Height (int for normal, float for high precision)");
}
