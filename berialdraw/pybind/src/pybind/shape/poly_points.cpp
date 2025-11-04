#include "pybind/pyberialdraw.hpp"
void bind_poly_points(py::module& m) {
    py::class_<berialdraw::PolyPoint, berialdraw::Point>(m, "PolyPoint")
        .def(py::init<berialdraw::Coord, berialdraw::Coord, bool>(),
             py::arg("x") = 0, py::arg("y") = 0, py::arg("pixel") = true,
             "Constructor")
        .def(py::init<const berialdraw::Point&>(),
             py::arg("p"),
             "Constructor from a Point object")
        .def_readwrite("next", &berialdraw::PolyPoint::next,
                      "Pointer to the next point in the polyline");

    py::class_<berialdraw::PolyPoints>(m, "PolyPoints")
        .def(py::init<>(),
             "Constructor")
        .def(py::init<const berialdraw::PolyPoints&>(),
             py::arg("other"),
             "Copy constructor")
        .def("append", py::overload_cast<const berialdraw::Point&>(&berialdraw::PolyPoints::append),
             py::arg("p"),
             "Append a Point object to the end of the polyline")
        .def("append", [](berialdraw::PolyPoints& self, int x, int y) {
            self.append(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Append point with integer coordinates")
        .def("append", [](berialdraw::PolyPoints& self, double x, double y) {
            self.append_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Append point with float coordinates (high precision)")
        
        .def("prepend", py::overload_cast<const berialdraw::Point&>(&berialdraw::PolyPoints::prepend),
             py::arg("p"),
             "Prepend a Point object to the start of the polyline")
        .def("prepend", [](berialdraw::PolyPoints& self, int x, int y) {
            self.prepend(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Prepend point with integer coordinates")
        .def("prepend", [](berialdraw::PolyPoints& self, double x, double y) {
            self.prepend_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Prepend point with float coordinates (high precision)")
        
        .def("clear", &berialdraw::PolyPoints::clear,
             "Clear the polyline, removing all points")
        .def("get_first", &berialdraw::PolyPoints::get_first,
             py::arg("p"),
             "Get the first point in the polyline")
        .def("get_next", &berialdraw::PolyPoints::get_next,
             py::arg("p"),
             "Get the next point in the polyline after a call to get_first")
        
        .def_property_readonly("count", &berialdraw::PolyPoints::count,
             "Get the number of points in the polyline");
}