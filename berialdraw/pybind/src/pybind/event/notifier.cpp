#include "pybind/pyberialdraw.hpp"

void bind_notifier(pybind11::module_& m) {
    // Bind LogEvent enum
    pybind11::enum_<berialdraw::Notifier::LogEvent>(m, "LogEvent")
        .value("LOG_NONE", berialdraw::Notifier::LogEvent::LOG_NONE)
        .value("LOG_CLICK", berialdraw::Notifier::LogEvent::LOG_CLICK)
        .value("LOG_MOVE", berialdraw::Notifier::LogEvent::LOG_MOVE)
        .value("LOG_KEY", berialdraw::Notifier::LogEvent::LOG_KEY)
        .value("LOG_SCROLL", berialdraw::Notifier::LogEvent::LOG_SCROLL)
        .value("LOG_USER", berialdraw::Notifier::LogEvent::LOG_USER);

    // Bind Notifier class
    pybind11::class_<berialdraw::Notifier>(m, "Notifier")
        .def(pybind11::init<>())
        .def("touch", static_cast<void(berialdraw::Notifier::*)(berialdraw::Coord, berialdraw::Coord, berialdraw::TouchEvent::TouchState)>(&berialdraw::Notifier::touch),
             pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("state"))
        .def("key", static_cast<void(berialdraw::Notifier::*)(wchar_t, berialdraw::KeyEvent::KeyState, berialdraw::KeyEvent::Modifier)>(&berialdraw::Notifier::key),
             pybind11::arg("key"), pybind11::arg("state"), pybind11::arg("modifier") = berialdraw::KeyEvent::MODIFIER_NONE)
        .def("scroll", static_cast<void(berialdraw::Notifier::*)(berialdraw::Coord, berialdraw::Coord, berialdraw::Widget*)>(&berialdraw::Notifier::scroll),
             pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("widget"))
        .def("click", static_cast<void(berialdraw::Notifier::*)(berialdraw::Coord, berialdraw::Coord, berialdraw::Widget*)>(&berialdraw::Notifier::click),
             pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("widget") = nullptr)
        .def("check", static_cast<void(berialdraw::Notifier::*)(bool, berialdraw::Widget*)>(&berialdraw::Notifier::check),
             pybind11::arg("state"), pybind11::arg("widget"))
        .def("slide", static_cast<void(berialdraw::Notifier::*)(int32_t, berialdraw::Widget*)>(&berialdraw::Notifier::slide),
             pybind11::arg("value"), pybind11::arg("widget"))
        .def("log", static_cast<void(berialdraw::Notifier::*)(berialdraw::Notifier::LogEvent)>(&berialdraw::Notifier::log),
             pybind11::arg("value") = berialdraw::Notifier::LogEvent::LOG_USER)
        .def("play_script", [](berialdraw::Notifier& self, const std::string& script_text, const std::string& snapshot_name) {
            // Convertir les std::string en berialdraw::String
            berialdraw::String script_str(script_text.c_str());
            berialdraw::String snapshot_str(snapshot_name.c_str());
            
            // Créer un TextStream temporaire à partir de la chaîne
            // Pour l'instant, on utilise une approche simplifiée
            // TODO: Implémenter une version qui accepte directement des strings
            // self.play_script(script_stream, snapshot_str);
        }, pybind11::arg("script_text"), pybind11::arg("snapshot_name"))
        .def("snapshot", [](berialdraw::Notifier& self, const std::string& filename) {
            // Convertir std::string en berialdraw::String
            berialdraw::String filename_str(filename.c_str());
            self.snapshot(filename_str);
        }, pybind11::arg("filename"));
}