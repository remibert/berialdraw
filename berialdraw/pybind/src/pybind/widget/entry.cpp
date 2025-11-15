#include "pybind/pyberialdraw.hpp"
void bind_entry(pybind11::module_& m) {
    // TypingMode enum
    pybind11::enum_<berialdraw::Entry::TypingMode>(m, "TypingMode")
        .value("INSERTION", berialdraw::Entry::TypingMode::INSERTION)
        .value("REPLACEMENT", berialdraw::Entry::TypingMode::REPLACEMENT);

    // Note: KeyEvent::Modifier is already bound in key_event.cpp with name "Modifier"
    // No need to rebind it here

    pybind11::class_<berialdraw::Entry>(m, "Entry")
        // Properties with explicit method casting
        .def_property("typing_mode", 
            static_cast<berialdraw::Entry::TypingMode (berialdraw::Entry::*)() const>(&berialdraw::Entry::typing_mode),
            static_cast<void (berialdraw::Entry::*)(berialdraw::Entry::TypingMode)>(&berialdraw::Entry::typing_mode))
        .def_property_readonly("cursor_position", &berialdraw::Entry::cursor_position)
        .def_property_readonly("selection_start", &berialdraw::Entry::selection_start)
        .def_property_readonly("selection_end", &berialdraw::Entry::selection_end)
        .def_property_readonly("input", &berialdraw::Entry::input, 
             pybind11::return_value_policy::reference_internal)
        .def_property("mask",
            static_cast<const berialdraw::String (berialdraw::Entry::*)() const>(&berialdraw::Entry::mask),
            static_cast<void (berialdraw::Entry::*)(const berialdraw::String&)>(&berialdraw::Entry::mask))
            
        // Methods
        .def("line_column", [](berialdraw::Entry& self) {
            uint32_t line, column;
            self.line_column(line, column);
            return pybind11::make_tuple(line, column);
        })
        .def("move_cursor", &berialdraw::Entry::move_cursor,
            pybind11::arg("line"), pybind11::arg("column"), pybind11::arg("selected") = false)
        .def("text_selected", [](berialdraw::Entry& self) {
            berialdraw::String text;
            self.text_selected(text);
            return text;
        })
        .def("select_text", &berialdraw::Entry::select_text,
            pybind11::arg("from"), pybind11::arg("to"))
        .def("select_all", &berialdraw::Entry::select_all)
        .def("select_none", &berialdraw::Entry::select_none)
        .def("append_key", &berialdraw::Entry::append_key)
        
        // Direct method for now (Entry has different signature than generic event system)
        .def("on_key_down", &berialdraw::Entry::on_key_down,
            pybind11::arg("key"), pybind11::arg("modifier"));
}