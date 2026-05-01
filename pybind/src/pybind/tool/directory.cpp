#include "pybind/pyberialdraw.hpp"
void bind_directory(py::module& m) {
    py::class_<berialdraw::Directory>(m, "Directory")
        .def(py::init<const char*>(),
             py::arg("path") = nullptr,
             "Constructor that initializes the directory with a given path")
        .def("open", &berialdraw::Directory::open,
             py::arg("path"),
             "Opens the directory specified in the constructor")
        .def("close", &berialdraw::Directory::close,
             "Closes the currently opened directory")
        .def("first", &berialdraw::Directory::first,
             "Resets to the first file in the directory")
        .def("next", &berialdraw::Directory::next,
             "Moves to the next file in the directory")
        .def("exist", &berialdraw::Directory::exist,
             "Indicates if file or directory existing")
        .def("match", &berialdraw::Directory::match,
             py::arg("pattern"), py::arg("ignore_case") = false,
             "Matches a string against a pattern containing wildcards")
        .def("file_size", &berialdraw::Directory::file_size,
             "Gets the size of the current file")
        .def("is_directory", &berialdraw::Directory::is_directory,
             "Checks if the current file is a directory")
        .def("is_file", &berialdraw::Directory::is_file,
             "Checks if the current file is a regular file")
        .def("filename", &berialdraw::Directory::filename,
             "Gets the name of the current file")
        .def("full_path", &berialdraw::Directory::full_path,
             "Gets the full path of the current file");
}
