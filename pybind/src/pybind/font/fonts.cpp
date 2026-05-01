#include "pybind/pyberialdraw.hpp"

void bind_fonts(py::module& m) {
    py::class_<berialdraw::Fonts>(m, "Fonts")
        .def(py::init<>(), "Create fonts")
        .def("load", &berialdraw::Fonts::load, py::arg("filename"), "Load font and add into the list")
        .def("unload", &berialdraw::Fonts::unload,  py::arg("filename"), "Unload font and remove from the list")
        .def("select", &berialdraw::Fonts::select,py::arg("familly"), py::arg("size"), py::arg("style") = berialdraw::Font::UNDEFINED,
             "Select the instance of font, if the font family not found return the first font")
        .def("load_directory", &berialdraw::Fonts::load_directory,
             py::arg("directory"),
             "Load all fonts from the selected directory")
        
        .def_property("familly",
            [](berialdraw::Fonts& self) -> std::string { return std::string(self.familly().c_str()); },
            [](berialdraw::Fonts& self, const std::string& value) { self.familly(value.c_str()); }, "Default font family");
}
