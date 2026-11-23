#include "pybind/pyberialdraw.hpp"

void bind_text_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::TextEvent, berialdraw::Event>(m, "TextEvent")
        .def("previous_text", [](const berialdraw::TextEvent& self) {
            return std::string(self.previous_text().c_str());
        })
        .def("text", [](const berialdraw::TextEvent& self) {
            return std::string(self.text().c_str());
        })
        .def_static("type_id", &berialdraw::TextEvent::type_id);
}
