#include "pybind/pyberialdraw.hpp"

void bind_clipboard(py::module& m) {
    // Bind ClipboardType enum
    py::enum_<berialdraw::ClipboardType>(m, "ClipboardType")
        .value("EMPTY", berialdraw::ClipboardType::EMPTY)
        .value("TEXT", berialdraw::ClipboardType::TEXT)
        .export_values();

    // Bind Clipboard class
    py::class_<berialdraw::Clipboard>(m, "Clipboard")
        .def(py::init<>(),
             "Constructor")
        .def("set_provider", &berialdraw::Clipboard::set_provider,
             py::arg("provider"),
             "Set clipboard provider for system clipboard operations")
        .def("provider", &berialdraw::Clipboard::provider,
             "Get clipboard provider")
        .def("copy_text", &berialdraw::Clipboard::copy_text,
             py::arg("text"),
             "Copy text to internal clipboard")
        .def("cut_text", &berialdraw::Clipboard::cut_text,
             py::arg("text"),
             "Cut text from internal clipboard")
        .def("paste_text", [](berialdraw::Clipboard& self) -> py::object {
            berialdraw::String text;
            if (self.paste_text(text)) {
                return py::cast(std::string(text.c_str()));
            } else {
                return py::none();
            }
        },
             "Paste text from internal clipboard, returns string or None")
        .def("type", &berialdraw::Clipboard::type,
             "Get clipboard content type")
        .def("clear", &berialdraw::Clipboard::clear,
             "Clear clipboard content")
        .def("is_empty", &berialdraw::Clipboard::is_empty,
             "Check if clipboard is empty")
        .def_property("text",
             [](berialdraw::Clipboard& self) -> py::object {
                 const berialdraw::String& text = self.text();
                 if (text.size() > 0) {
                     return py::cast(std::string(text.c_str()));
                 } else {
                     return py::none();
                 }
             },
             [](berialdraw::Clipboard& self, const std::string& value) {
                 self.text(berialdraw::String(value.c_str()));
             },
             "Get/Set clipboard text content as a property")
        .def("sync_from_system", &berialdraw::Clipboard::sync_from_system,
             "Synchronize clipboard from system (for external changes), returns bool");
}
