#include "pybind/pyberialdraw.hpp"

void bind_clipboard(py::module& m) {
	py::enum_<berialdraw::ClipboardType>(m, "ClipboardType")
		.value("TEXT", berialdraw::ClipboardType::TEXT,
			   "Text data type")
		.value("EMPTY", berialdraw::ClipboardType::EMPTY,
			   "Empty clipboard");

	py::class_<berialdraw::Clipboard>(m, "Clipboard")
		.def(py::init<>(),
			 "Constructor")
		.def("copy_text", &berialdraw::Clipboard::copy_text,
			 py::arg("text"),
			 "Copy text to internal clipboard")
		.def("cut_text", &berialdraw::Clipboard::cut_text,
			 py::arg("text"),
			 "Cut text from internal clipboard (copy then clear)")
		.def("paste_text", [](berialdraw::Clipboard& self) {
				berialdraw::String text;
				bool result = self.paste_text(text);
				return py::make_tuple(result, text.c_str());
			 },
			 "Paste text from internal clipboard, returns (success, text)")
		.def("type", &berialdraw::Clipboard::type,
			 "Get clipboard content type")
		.def("clear", &berialdraw::Clipboard::clear,
			 "Clear clipboard content")
		.def("is_empty", &berialdraw::Clipboard::is_empty,
			 "Check if clipboard is empty")
		.def("text", static_cast<const berialdraw::String& (berialdraw::Clipboard::*)() const>(&berialdraw::Clipboard::text),
			 "Get text content of clipboard")
		.def("text", static_cast<void (berialdraw::Clipboard::*)(const berialdraw::String&)>(&berialdraw::Clipboard::text),
			 py::arg("str"),
			 "Set text content of clipboard")
		.def("sync_from_system", &berialdraw::Clipboard::sync_from_system,
			 "Synchronize clipboard from system (for external changes), returns true if updated");
}
