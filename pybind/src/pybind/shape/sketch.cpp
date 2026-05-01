#include "pybind/pyberialdraw.hpp"
void bind_sketch(py::module& m) {
    py::class_<berialdraw::Path>(m, "Path")
        .def(py::init<void*>(),
             py::arg("parent"),
             "Constructor")
        .def_property("path", 
            py::overload_cast<>(&berialdraw::Path::path, py::const_),
            py::overload_cast<const berialdraw::String&>(&berialdraw::Path::path),
            "Path value")
        .def_property("color", 
            py::overload_cast<>(&berialdraw::Path::color, py::const_),
            [](berialdraw::Path& self, py::object value) {
                if (py::isinstance<py::tuple>(value)) {
                    auto tuple = value.cast<py::tuple>();
                    if (tuple.size() == 2) {
                        self.color(tuple[0].cast<uint32_t>(), tuple[1].cast<uint8_t>());
                    }
                } else {
                    self.color(value.cast<uint32_t>());
                }
            }, "Color value (int for RGB, tuple(color, alpha) for RGBA)");

    py::class_<berialdraw::Sketch, berialdraw::Shape>(m, "Sketch")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for sketch")
        .def(py::init<berialdraw::Widget*>(),
             py::arg("widget"),
             "Constructor for sketch")
        .def(py::init<const berialdraw::Sketch&>(),
             py::arg("other"),
             "Copy constructor for sketch")
        .def("add_path", &berialdraw::Sketch::add_path,
             py::arg("color"), py::arg("path"),
             "Add bezier path")
        .def("clear_paths", &berialdraw::Sketch::clear_paths,
             "Clear all bezier path")
        
        .def_property("resolution", 
            py::overload_cast<>(&berialdraw::Sketch::resolution, py::const_),
            [](berialdraw::Sketch& self, py::object value) {
                if (py::isinstance<py::tuple>(value)) {
                    auto tuple = value.cast<py::tuple>();
                    if (tuple.size() == 2) {
                        if (py::isinstance<py::int_>(tuple[0])) {
                            self.resolution(tuple[0].cast<int>(), tuple[1].cast<int>());
                        } else {
                            self.resolution_(static_cast<int>(tuple[0].cast<double>() * 64), 
                                           static_cast<int>(tuple[1].cast<double>() * 64));
                        }
                    }
                } else if (py::isinstance<berialdraw::Size>(value)) {
                    self.resolution(value.cast<berialdraw::Size>());
                } else if (py::isinstance<py::int_>(value)) {
                    self.resolution(value.cast<int>());
                } else {
                    self.resolution_(static_cast<int>(value.cast<double>() * 64));
                }
            }, "Resolution (Size object, int, float for high precision, or tuple(w,h))")
            
        .def_property("zoom", 
            py::overload_cast<>(&berialdraw::Sketch::zoom, py::const_),
            [](berialdraw::Sketch& self, py::object value) {
                if (py::isinstance<py::int_>(value)) {
                    self.zoom(value.cast<int>());
                } else if (py::isinstance<py::float_>(value)) {
                    self.zoom_(static_cast<int>(value.cast<double>() * 64));
                }
            }, "Zoom ratio (int for normal precision, float for high precision)")
            
        .def_property("filename", 
            py::overload_cast<>(&berialdraw::Sketch::filename, py::const_),
            py::overload_cast<const berialdraw::String&>(&berialdraw::Sketch::filename),
            "Filename value")
            
        .def("load", &berialdraw::Sketch::load,
             "Load the icon from filename");
}