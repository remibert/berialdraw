#include "pybind/pyberialdraw.hpp"
void bind_size(py::module& m) {
    auto cls = py::class_<berialdraw::Size>(m, "Size")
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
        .def("set", [](berialdraw::Size& self, double w, double h) { self.set_q6(to_q6(w), to_q6(h)); },
             py::arg("w"), py::arg("h"),
             "Set size with float values (high precision)")
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
        .def("decrease", [](berialdraw::Size& self, double w, double h) { self.decrease_q6(to_q6(w), to_q6(h)); },
             py::arg("w"), py::arg("h"),
             "Decrease size with float values (high precision)")
        .def("increase", [](berialdraw::Size& self, double w, double h) { self.increase_q6(to_q6(w), to_q6(h)); },
             py::arg("w"), py::arg("h"),
             "Increase size with float values (high precision)")
        .def("nearest_pixel", &berialdraw::Size::nearest_pixel,
             "Resizes itself on the nearest pixel")
        .def("is_width_undefined", &berialdraw::Size::is_width_undefined,
             "Indicates if width is not defined")
        .def("is_height_undefined", &berialdraw::Size::is_height_undefined,
             "Indicates if height is not defined")
        .def("clean", &berialdraw::Size::clean,
             "Clean the size and set to undefined")
        .def("print", &berialdraw::Size::print,
             py::arg("name"), py::arg("newline") = false,
             "Print content");
             
    // Propriétés avec précision automatique
    bind_precision_property(cls, "width",
        &berialdraw::Size::width,
        &berialdraw::Size::width,
        &berialdraw::Size::width_q6,
        "Width (int for normal, float for high precision)");
    bind_precision_property(cls, "height",
        &berialdraw::Size::height,
        &berialdraw::Size::height,
        &berialdraw::Size::height_q6,
        "Height (int for normal, float for high precision)");
}
