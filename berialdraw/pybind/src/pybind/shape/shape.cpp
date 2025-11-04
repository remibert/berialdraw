#include "pybind/pyberialdraw.hpp"
void bind_shape(py::module& m) {
    py::class_<berialdraw::Shape, berialdraw::CommonStyle>(m, "Shape")
        .def("paint", py::overload_cast<const berialdraw::Point&>(&berialdraw::Shape::paint),
             py::arg("shift"),
             "Paints the shape on the desktop with a shift")
        .def("polygon", &berialdraw::Shape::polygon,
             py::return_value_policy::reference_internal,
             "Get the polygon representing the shape");
}