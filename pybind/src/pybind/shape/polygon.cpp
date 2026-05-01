#include "pybind/pyberialdraw.hpp"
void bind_polygon(py::module& m) {
    py::class_<berialdraw::Polygon>(m, "Polygon")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor")
        .def(py::init<const berialdraw::Polygon&>(),
             py::arg("other"),
             "Copy constructor")

        .def("add_point", py::overload_cast<const berialdraw::Point&>(&berialdraw::Polygon::add_point),
             py::arg("p"),
             "Add a point to the polygon")
        .def("add_point", [](berialdraw::Polygon& self, int x, int y) {
            self.add_point(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Add point with integer coordinates")
        .def("add_point", [](berialdraw::Polygon& self, double x, double y) {
            self.add_point_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Add point with float coordinates (high precision)")
        
        .def("add_cubic", py::overload_cast<const berialdraw::Point&>(&berialdraw::Polygon::add_cubic),
             py::arg("p"),
             "Add a cubic bezier curve to the polygon")
        .def("add_cubic", [](berialdraw::Polygon& self, int x, int y) {
            self.add_cubic(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Add cubic curve with integer coordinates")
        .def("add_cubic", [](berialdraw::Polygon& self, double x, double y) {
            self.add_cubic_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Add cubic curve with float coordinates (high precision)")
        
        .def("add_conic", py::overload_cast<const berialdraw::Point&>(&berialdraw::Polygon::add_conic),
             py::arg("p"),
             "Add a conic bezier curve to the polygon")
        .def("add_conic", [](berialdraw::Polygon& self, int x, int y) {
            self.add_conic(x, y);  // Version entière normale
        }, py::arg("x"), py::arg("y"), "Add conic curve with integer coordinates")
        .def("add_conic", [](berialdraw::Polygon& self, double x, double y) {
            self.add_conic_(static_cast<berialdraw::Coord>(x * 64), static_cast<berialdraw::Coord>(y * 64));  // Version float avec précision
        }, py::arg("x"), py::arg("y"), "Add conic curve with float coordinates (high precision)")
        .def("next_contour", &berialdraw::Polygon::next_contour,
             "Select the next contour")
        .def("clear", &berialdraw::Polygon::clear,
             "Clear the polygon, removing all points")
             
        .def_property("zoom", 
            py::overload_cast<>(&berialdraw::Polygon::zoom),
            [](berialdraw::Polygon& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.zoom(value.cast<int>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.zoom_(static_cast<int>(value.cast<double>() * 64));
                }
            }, "Zoom ratio (int for normal precision, float for high precision)")
        .def("polygon", &berialdraw::Polygon::polygon,
             "Get the polygon object",
             py::return_value_policy::reference_internal);

}
