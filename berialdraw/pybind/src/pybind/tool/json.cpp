#include "pybind/pyberialdraw.hpp"

void bind_json(py::module& m) {
    py::class_<berialdraw::Json>(m, "Json")
        .def(py::init<>(),
             "Constructor - creates an empty JSON object")
        .def(py::init<const char*>(),
             py::arg("buffer"),
             "Constructor - creates JSON from buffer")
        // Note: berialdraw::Json has a copy constructor taking Json& (non-const).
        // pybind11 cannot bind a constructor with a non-const reference from a const arg directly,
        // so provide a factory that accepts a Python Json instance and uses the non-const copy ctor.
        .def_static("from_json", [](py::object obj) {
                berialdraw::Json & other = obj.cast<berialdraw::Json&>();
                berialdraw::String buf;
                other.serialize(buf, -1);
                return berialdraw::Json(buf.c_str());
            },
            py::arg("json"),
            "Create a Json by copying an existing Json instance")
        .def("serialize", [](berialdraw::Json& self, int32_t indent) {
                berialdraw::String result;
                self.serialize(result, indent);
                return (std::string)(const char*)result.c_str();
             },
             py::arg("indent") = -1,
             "Serialize JSON to string")
        .def("unserialize", [](berialdraw::Json& self, const std::string& buffer) {
                self.unserialize(buffer.c_str());
             },
             py::arg("buffer"),
             "Deserialize JSON from string")
        .def("clear", &berialdraw::Json::clear,
             "Clear all JSON content");
}
