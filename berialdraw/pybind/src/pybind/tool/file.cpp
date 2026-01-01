#include "pybind/pyberialdraw.hpp"
void bind_file(py::module& m) {
    py::class_<berialdraw::File>(m, "File")
        .def(py::init<>(),
             "Default constructor")
        .def("open", &berialdraw::File::open,
             py::arg("pathname"), py::arg("mode"),
             "Open file with mode")
        .def("close", &berialdraw::File::close,
             "Close file")
        .def("read", [](berialdraw::File& self, uint32_t size) {
                std::vector<uint8_t> buffer(size);
                int bytes_read = self.read(buffer.data(), size);
                if (bytes_read < 0) return py::bytes();
                return py::bytes(reinterpret_cast<const char*>(buffer.data()), static_cast<size_t>(bytes_read));
             },
             py::arg("size"),
             "Read data from the stream")
        .def("write", [](berialdraw::File& self, const py::bytes& data) {
                std::string str = data;
                return self.write(str.data(), static_cast<uint32_t>(str.size()));
             },
             py::arg("data"),
             "Write data into the stream")
        .def("tell", &berialdraw::File::tell,
             "Give the position into stream")
        .def("seek", &berialdraw::File::seek,
             py::arg("offset"), py::arg("whence"),
             "Repositions the offset in the stream")
        .def("size", &berialdraw::File::size,
             "Return the total size of the stream")
        .def("write_string", &berialdraw::File::write_string,
             py::arg("buffer"),
             "Write a string buffer to the end of the current text stream")
        .def("write_char", &berialdraw::File::write_char,
             py::arg("character"),
             "Write a wide character to the end of the current text stream")
        .def("clear", &berialdraw::File::clear,
             "Clear the content of stream")
        .def("offset", static_cast<uint32_t (berialdraw::File::*)()>(&berialdraw::File::offset),
             "Get the current offset in buffer")
        .def("offset", static_cast<void (berialdraw::File::*)(uint32_t)>(&berialdraw::File::offset),
             py::arg("val"),
             "Set the offset in buffer")
        .def("read_char", &berialdraw::File::read_char,
             "Read the wide character next")
        .def_static("exists", &berialdraw::File::exists,
                   py::arg("file_name"),
                   "Indicates if the files exists")
        .def_static("resource_dir", static_cast<berialdraw::String (*)(const berialdraw::String&)>(&berialdraw::File::resource_dir),
                   py::arg("dir"),
                   "Set the resource directory and return the resource directory existing");
}
