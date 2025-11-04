#include "pybind/pyberialdraw.hpp"
void bind_key_event(pybind11::module_& m) {
    // KeyState enum
    pybind11::enum_<berialdraw::KeyEvent::KeyState>(m, "KeyState")
        .value("KEY_UP", berialdraw::KeyEvent::KEY_UP)
        .value("KEY_DOWN", berialdraw::KeyEvent::KEY_DOWN);

    // Modifier enum
    pybind11::enum_<berialdraw::KeyEvent::Modifier>(m, "Modifier")
        .value("MODIFIER_NONE", berialdraw::KeyEvent::MODIFIER_NONE)
        .value("MODIFIER_SHIFT", berialdraw::KeyEvent::MODIFIER_SHIFT)
        .value("MODIFIER_CONTROL", berialdraw::KeyEvent::MODIFIER_CONTROL)
        .value("MODIFIER_ALT", berialdraw::KeyEvent::MODIFIER_ALT);

    pybind11::class_<berialdraw::KeyEvent, berialdraw::Event>(m, "KeyEvent")
        .def(pybind11::init<wchar_t, berialdraw::KeyEvent::KeyState, berialdraw::KeyEvent::Modifier>(),
            pybind11::arg("key"), pybind11::arg("state"), pybind11::arg("modifier") = berialdraw::KeyEvent::MODIFIER_NONE)
        .def_property_readonly("key", &berialdraw::KeyEvent::key)
        .def_property_readonly("state", &berialdraw::KeyEvent::state)
        .def_property_readonly("modifier", &berialdraw::KeyEvent::modifier);
}