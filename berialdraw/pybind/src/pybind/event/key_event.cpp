#include "pybind/pyberialdraw.hpp"
#include "styles/keys.hpp"

void bind_key_event(pybind11::module_& m) {
    // Créer ReservedKey comme IntEnum pour que les comparaisons avec int fonctionnent
    pybind11::dict reserved_key_values;
    reserved_key_values["KEY_LETTER"] = (int)berialdraw::ReservedKey::KEY_LETTER;
    reserved_key_values["KEY_NUMERIC"] = (int)berialdraw::ReservedKey::KEY_NUMERIC;
    reserved_key_values["KEY_SHIFT"] = (int)berialdraw::ReservedKey::KEY_SHIFT;
    reserved_key_values["KEY_ENTER"] = (int)berialdraw::ReservedKey::KEY_ENTER;
    reserved_key_values["KEY_ESCAPE"] = (int)berialdraw::ReservedKey::KEY_ESCAPE;
    reserved_key_values["KEY_BACKSPACE"] = (int)berialdraw::ReservedKey::KEY_BACKSPACE;
    reserved_key_values["KEY_TABULATION"] = (int)berialdraw::ReservedKey::KEY_TABULATION;
    reserved_key_values["KEY_DELETE"] = (int)berialdraw::ReservedKey::KEY_DELETE;
    reserved_key_values["KEY_CTRL_A"] = (int)berialdraw::ReservedKey::KEY_CTRL_A;
    reserved_key_values["KEY_CTRL_B"] = (int)berialdraw::ReservedKey::KEY_CTRL_B;
    reserved_key_values["KEY_CTRL_C"] = (int)berialdraw::ReservedKey::KEY_CTRL_C;
    reserved_key_values["KEY_CTRL_D"] = (int)berialdraw::ReservedKey::KEY_CTRL_D;
    reserved_key_values["KEY_CTRL_E"] = (int)berialdraw::ReservedKey::KEY_CTRL_E;
    reserved_key_values["KEY_CTRL_F"] = (int)berialdraw::ReservedKey::KEY_CTRL_F;
    reserved_key_values["KEY_CTRL_G"] = (int)berialdraw::ReservedKey::KEY_CTRL_G;
    reserved_key_values["KEY_CTRL_H"] = (int)berialdraw::ReservedKey::KEY_CTRL_H;
    reserved_key_values["KEY_CTRL_I"] = (int)berialdraw::ReservedKey::KEY_CTRL_I;
    reserved_key_values["KEY_CTRL_J"] = (int)berialdraw::ReservedKey::KEY_CTRL_J;
    reserved_key_values["KEY_CTRL_K"] = (int)berialdraw::ReservedKey::KEY_CTRL_K;
    reserved_key_values["KEY_CTRL_L"] = (int)berialdraw::ReservedKey::KEY_CTRL_L;
    reserved_key_values["KEY_CTRL_M"] = (int)berialdraw::ReservedKey::KEY_CTRL_M;
    reserved_key_values["KEY_CTRL_N"] = (int)berialdraw::ReservedKey::KEY_CTRL_N;
    reserved_key_values["KEY_CTRL_O"] = (int)berialdraw::ReservedKey::KEY_CTRL_O;
    reserved_key_values["KEY_CTRL_P"] = (int)berialdraw::ReservedKey::KEY_CTRL_P;
    reserved_key_values["KEY_CTRL_Q"] = (int)berialdraw::ReservedKey::KEY_CTRL_Q;
    reserved_key_values["KEY_CTRL_R"] = (int)berialdraw::ReservedKey::KEY_CTRL_R;
    reserved_key_values["KEY_CTRL_S"] = (int)berialdraw::ReservedKey::KEY_CTRL_S;
    reserved_key_values["KEY_CTRL_T"] = (int)berialdraw::ReservedKey::KEY_CTRL_T;
    reserved_key_values["KEY_CTRL_U"] = (int)berialdraw::ReservedKey::KEY_CTRL_U;
    reserved_key_values["KEY_CTRL_V"] = (int)berialdraw::ReservedKey::KEY_CTRL_V;
    reserved_key_values["KEY_CTRL_W"] = (int)berialdraw::ReservedKey::KEY_CTRL_W;
    reserved_key_values["KEY_CTRL_X"] = (int)berialdraw::ReservedKey::KEY_CTRL_X;
    reserved_key_values["KEY_CTRL_Y"] = (int)berialdraw::ReservedKey::KEY_CTRL_Y;
    reserved_key_values["KEY_CTRL_Z"] = (int)berialdraw::ReservedKey::KEY_CTRL_Z;
    reserved_key_values["KEY_LEFT"] = (int)berialdraw::ReservedKey::KEY_LEFT;
    reserved_key_values["KEY_UP"] = (int)berialdraw::ReservedKey::KEY_UP;
    reserved_key_values["KEY_RIGHT"] = (int)berialdraw::ReservedKey::KEY_RIGHT;
    reserved_key_values["KEY_DOWN"] = (int)berialdraw::ReservedKey::KEY_DOWN;
    reserved_key_values["KEY_HOME"] = (int)berialdraw::ReservedKey::KEY_HOME;
    reserved_key_values["KEY_END"] = (int)berialdraw::ReservedKey::KEY_END;
    reserved_key_values["KEY_PAUSE"] = (int)berialdraw::ReservedKey::KEY_PAUSE;
    reserved_key_values["KEY_EXIT"] = (int)berialdraw::ReservedKey::KEY_EXIT;

    auto int_enum = pybind11::module_::import("enum").attr("IntEnum");
    m.attr("ReservedKey") = int_enum("ReservedKey", reserved_key_values);

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
        .def_property_readonly("key", [](const berialdraw::KeyEvent& self) { return static_cast<int>(self.key()); })
        .def_property_readonly("state", &berialdraw::KeyEvent::state)
        .def_property_readonly("modifier", &berialdraw::KeyEvent::modifier);
}