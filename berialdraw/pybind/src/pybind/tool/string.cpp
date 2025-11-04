#include "pybind/pyberialdraw.hpp"
void bind_string(py::module& m) {
    py::class_<berialdraw::String>(m, "String")
        .def(py::init<>(),
             "Constructor")
        .def(py::init<const berialdraw::String&>(),
             py::arg("string"),
             "Copy constructor")
        .def(py::init<const char*>(),
             py::arg("string"),
             "Constructor with string buffer")
        .def("__str__", &berialdraw::String::c_str,
             "Cast to string")
        .def("__eq__", py::overload_cast<const berialdraw::String&>(&berialdraw::String::operator==, py::const_),
             py::arg("other"),
             "Compare if equal string")
        .def("__eq__", py::overload_cast<const char*>(&berialdraw::String::operator==, py::const_),
             py::arg("other"),
             "Compare if equal string buffer")
        .def("__ne__", py::overload_cast<const berialdraw::String&>(&berialdraw::String::operator!=, py::const_),
             py::arg("other"),
             "Compare if different string")
        .def("__ne__", py::overload_cast<const char*>(&berialdraw::String::operator!=, py::const_),
             py::arg("other"),
             "Compare if different string buffer")
        .def("__add__", py::overload_cast<const char*>(&berialdraw::String::operator+, py::const_),
             py::arg("other"),
             "Concatenate two strings")
        .def("__add__", py::overload_cast<const berialdraw::String&>(&berialdraw::String::operator+, py::const_),
             py::arg("other"),
             "Concatenate two strings")
        .def("__add__", py::overload_cast<wchar_t>(&berialdraw::String::operator+, py::const_),
             py::arg("other"),
             "Concatenate character to string")
        .def("__iadd__", py::overload_cast<const char*>(&berialdraw::String::operator+=),
             py::arg("other"),
             "Append to current string")
        .def("__iadd__", py::overload_cast<const berialdraw::String&>(&berialdraw::String::operator+=),
             py::arg("other"),
             "Append to current string")
        .def("__iadd__", py::overload_cast<wchar_t>(&berialdraw::String::operator+=),
             py::arg("other"),
             "Append character to current string")
        .def("find", &berialdraw::String::find,
             py::arg("str"), py::arg("pos") = 0,
             "Find string in the string")
        .def("rfind", &berialdraw::String::rfind,
             py::arg("str"), py::arg("pos") = -1,
             "Find string in the string in reverse")
        .def("c_str", &berialdraw::String::c_str,
             "Return the string buffer pointer")
        .def("size", &berialdraw::String::size,
             "Get size in bytes of the string buffer")
        .def("count", &berialdraw::String::count,
             "Get the quantity of wide characters in the string")
        .def("get", &berialdraw::String::get,
             py::arg("index"),
             "Get the wide character at the index")
        .def("print", [](berialdraw::String& self, const std::string& format) {
                self.print("%s", format.c_str());
             },
             py::arg("format"),
             "Printf into string clear previous string")
        .def("insert", py::overload_cast<wchar_t, int32_t>(&berialdraw::String::insert),
             py::arg("character"), py::arg("index"),
             "Insert character at position")
        .def("insert", py::overload_cast<const char*, int32_t>(&berialdraw::String::insert),
             py::arg("string"), py::arg("index"),
             "Insert string at position")
        .def("insert", py::overload_cast<const berialdraw::String&, int32_t>(&berialdraw::String::insert),
             py::arg("string"), py::arg("index"),
             "Insert string at position")
        .def("replace", &berialdraw::String::replace,
             py::arg("character"), py::arg("index"),
             "Replace character at position")
        .def("remove", py::overload_cast<int32_t>(&berialdraw::String::remove),
             py::arg("index"),
             "Remove character at position")
        .def("slice", [](const berialdraw::String& self, int32_t start, int32_t end) {
                berialdraw::String part;
                self.slice(start, end, part);
                return part;
             },
             py::arg("start"), py::arg("end"),
             "Get a slice of string")
        .def("remove", py::overload_cast<int32_t, int32_t>(&berialdraw::String::remove),
             py::arg("start"), py::arg("end"),
             "Remove a slice of string")
        .def("to_int", [](const berialdraw::String& self) -> py::object {
                int32_t value;
                if (self.to_int(value)) {
                    return py::cast(value);
                }
                return py::none();
             },
             "Convert to integer")
        .def("to_int64", [](const berialdraw::String& self) -> py::object {
                int64_t value;
                if (self.to_int(value)) {
                    return py::cast(value);
                }
                return py::none();
             },
             "Convert to 64-bit integer")
        .def("write_char", &berialdraw::String::write_char,
             py::arg("character"),
             "Write a wide character to the end of the current text stream")
        .def("write_string", &berialdraw::String::write_string,
             py::arg("buffer"),
             "Write a string buffer to the end of the current text stream")
        .def("clear", &berialdraw::String::clear,
             "Clear the content of stream")
        .def("offset", py::overload_cast<>(&berialdraw::String::offset),
             "Get the current offset in bytes into the string buffer")
        .def("offset", py::overload_cast<uint32_t>(&berialdraw::String::offset),
             py::arg("val"),
             "Set the offset in bytes into the string buffer")
        .def("read_char", &berialdraw::String::read_char,
             "Read the wide character next")
        .def_static("to_int_static", py::overload_cast<const char*, int&>(&berialdraw::String::to_int),
                   py::arg("str"), py::arg("value"),
                   "Convert string to integer")
        .def_static("to_int64_static", py::overload_cast<const char*, long long&>(&berialdraw::String::to_int),
                   py::arg("str"), py::arg("value"),
                   "Convert string to 64-bit integer")
        .def_readonly_static("empty", &berialdraw::String::empty, "Empty string");

    // Convertisseur automatique de const char* vers berialdraw::String
    py::implicitly_convertible<const char*, berialdraw::String>();
}
