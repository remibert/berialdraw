#include "pybind/pyberialdraw.hpp"
void bind_image(py::module& m) {
    py::class_<berialdraw::Image, berialdraw::Shape>(m, "Image")
        .def(py::init<berialdraw::Canvas*>(),
             py::arg("canvas"),
             "Constructor for image on canvas")
        .def(py::init<berialdraw::Widget*>(),
             py::arg("widget"),
             "Constructor for image on widget")
        .def(py::init<const berialdraw::Image&>(),
             py::arg("other"),
             "Copy constructor for image")
        
        .def_property("filename", 
            py::overload_cast<>(&berialdraw::Image::filename, py::const_),
            py::overload_cast<const berialdraw::String&>(&berialdraw::Image::filename),
            "Filename value")
            
        .def_property("fit_mode", 
            py::overload_cast<>(&berialdraw::Image::fit_mode, py::const_),
            py::overload_cast<berialdraw::ImageFitMode>(&berialdraw::Image::fit_mode),
            "Fit mode for image")
            
        .def_property("alpha", 
            py::overload_cast<>(&berialdraw::Image::alpha, py::const_),
            py::overload_cast<uint8_t>(&berialdraw::Image::alpha),
            "Alpha transparency (0=transparent, 255=opaque)")
            
        .def("image_width", &berialdraw::Image::image_width,
             "Get the image width in pixels")
        .def("image_height", &berialdraw::Image::image_height,
             "Get the image height in pixels")
        .def("is_loaded", &berialdraw::Image::is_loaded,
             "Check if the image is loaded")
        .def("load", &berialdraw::Image::load,
             "Load the image from filename");
}
