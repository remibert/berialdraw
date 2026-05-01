#include "pybind/pyberialdraw.hpp"
void bind_poly_lines(py::module& m) {
    py::class_<berialdraw::PolyLines, berialdraw::Shape, berialdraw::RoundStyle>(m, "PolyLines")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::PolyLines&>(),
             py::arg("other"),
             "Copy constructor")
        .def("prepend", py::overload_cast<const berialdraw::Point&>(&berialdraw::PolyLines::prepend),
             py::arg("p"),
             "Prepend a point to the polyline")
        .def("prepend", [](berialdraw::PolyLines& self, int x, int y) {
            self.prepend(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Prepend point with integer coordinates")
        .def("prepend", [](berialdraw::PolyLines& self, double x, double y) {
            self.prepend_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Prepend point with float coordinates (high precision)")
        
        .def("append", py::overload_cast<const berialdraw::Point&>(&berialdraw::PolyLines::append),
             py::arg("p"),
             "Append a point to the polyline")
        .def("append", [](berialdraw::PolyLines& self, int x, int y) {
            self.append(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Append point with integer coordinates")
        .def("append", [](berialdraw::PolyLines& self, double x, double y) {
            self.append_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Append point with float coordinates (high precision)")
        
        .def("clear", &berialdraw::PolyLines::clear,
             "Clear the polyline, removing all points");
}