#include "pybind/pyberialdraw.hpp"
void bind_illustration(py::module& m) {
    py::class_<berialdraw::Illustration, berialdraw::Shape>(m, "Illustration")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for illustration on canvas")
        .def(py::init<berialdraw::Widget*>(),
             py::arg("widget"),
             "Constructor for illustration on widget")
        .def(py::init<const berialdraw::Illustration&>(),
             py::arg("other"),
             "Copy constructor for illustration")
        
        .def_property("filename", 
            py::overload_cast<>(&berialdraw::Illustration::filename, py::const_),
            py::overload_cast<const berialdraw::String&>(&berialdraw::Illustration::filename),
            "Filename value (auto-detects PNG/JPEG vs ICN/SVG)")
            
        .def_property("fit_mode", 
            py::overload_cast<>(&berialdraw::Illustration::fit_mode, py::const_),
            py::overload_cast<berialdraw::ImageFitMode>(&berialdraw::Illustration::fit_mode),
            "Fit mode for illustration")
            
        .def_property("alpha", 
            py::overload_cast<>(&berialdraw::Illustration::alpha, py::const_),
            py::overload_cast<uint8_t>(&berialdraw::Illustration::alpha),
            "Alpha transparency (0=transparent, 255=opaque)")
            
        .def("is_vector", &berialdraw::Illustration::is_vector,
             "Check if illustration is vector (true) or raster (false)")
        .def("is_loaded", &berialdraw::Illustration::is_loaded,
             "Check if the illustration is loaded")
        .def("image_width", &berialdraw::Illustration::image_width,
             "Get the illustration width in pixels")
        .def("image_height", &berialdraw::Illustration::image_height,
             "Get the illustration height in pixels")
        .def("load", &berialdraw::Illustration::load,
             "Load the illustration from filename");
}
