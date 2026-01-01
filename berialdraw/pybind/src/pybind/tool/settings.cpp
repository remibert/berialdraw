#include "pybind/pyberialdraw.hpp"

void bind_settings(py::module& m) {
    py::class_<berialdraw::Settings, berialdraw::Json>(m, "Settings")
        .def(py::init<>(),
             "Constructor - creates an empty settings object")
        .def(py::init<const char*>(),
             py::arg("buffer"),
             "Constructor - creates settings from JSON buffer")
        .def(py::init<const berialdraw::Settings&>(),
             py::arg("settings"),
             "Copy constructor")
        .def("resolve", [](berialdraw::Settings& self, const std::string& template_str) {
                return (std::string)(berialdraw::String)self.resolve(template_str.c_str());
             },
             py::arg("template_str"),
             "Resolve a string by replacing ${name} placeholders with setting values")
        .def("set_bool", [](berialdraw::Settings& self, const std::string& name, bool value) {
                berialdraw::JsonIterator it(self);
                it[name.c_str()] = value;
             },
             py::arg("name"), py::arg("value"),
             "Set a boolean value")
        .def("set_int", [](berialdraw::Settings& self, const std::string& name, int32_t value) {
                berialdraw::JsonIterator it(self);
                it[name.c_str()] = value;
             },
             py::arg("name"), py::arg("value"),
             "Set an integer value")
        .def("set_string", [](berialdraw::Settings& self, const std::string& name, const std::string& value) {
                berialdraw::JsonIterator it(self);
                it[name.c_str()] = value.c_str();
             },
             py::arg("name"), py::arg("value"),
             "Set a string value")
        .def("get_bool", [](berialdraw::Settings& self, const std::string& name, bool default_val) {
                berialdraw::JsonIterator it(self);
                return (bool)(it[name.c_str()] | default_val);
             },
             py::arg("name"), py::arg("default_val") = false,
             "Get a boolean value with default")
        .def("get_int", [](berialdraw::Settings& self, const std::string& name, int32_t default_val) {
                berialdraw::JsonIterator it(self);
                return (int32_t)(it[name.c_str()] | default_val);
             },
             py::arg("name"), py::arg("default_val") = 0,
             "Get an integer value with default")
        .def("get_string", [](berialdraw::Settings& self, const std::string& name, const std::string& default_val) {
                berialdraw::JsonIterator it(self);
                berialdraw::String def(default_val.c_str());
                return (std::string)(berialdraw::String)(it[name.c_str()] | def);
             },
             py::arg("name"), py::arg("default_val") = "",
             "Get a string value with default")
        .def("serialize", [](berialdraw::Settings& self, int32_t indent) {
                berialdraw::String result;
                self.serialize(result, indent);
                return (std::string)(const char*)result.c_str();
             },
             py::arg("indent") = -1,
             "Serialize settings to JSON string")
        .def("unserialize", [](berialdraw::Settings& self, const std::string& buffer) {
                self.unserialize(buffer.c_str());
             },
             py::arg("buffer"),
             "Deserialize settings from JSON string");
}
