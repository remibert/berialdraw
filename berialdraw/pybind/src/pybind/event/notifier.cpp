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
        .def("play_script", [](berialdraw::Notifier& self, const pybind11::object& script_input, const pybind11::object& snapshot_name = pybind11::none()) {
            berialdraw::String script_str;
            
            // Handle script_input as either string or list of dicts
            if (pybind11::isinstance<pybind11::str>(script_input)) {
                // Direct JSON string
                script_str = py_to_string(script_input);
            } else if (pybind11::isinstance<pybind11::list>(script_input)) {
                // Convert list of dicts to JSON string
                auto script_list = pybind11::cast<pybind11::list>(script_input);
                
                // Build JSON array string
                std::string json_str = "[";
                for (size_t i = 0; i < pybind11::len(script_list); ++i) {
                    if (i > 0) json_str += ",";
                    
                    auto dict = pybind11::cast<pybind11::dict>(script_list[i]);
                    json_str += "{";
                    
                    bool first = true;
                    for (auto item : dict) {
                        if (!first) json_str += ",";
                        first = false;
                        
                        auto key = pybind11::cast<std::string>(item.first);
                        json_str += "\"" + key + "\":";
                        
                        auto value = item.second;
                        if (pybind11::isinstance<pybind11::str>(value)) {
                            auto str_val = pybind11::cast<std::string>(value);
                            json_str += "\"" + str_val + "\"";
                        } else if (pybind11::isinstance<pybind11::int_>(value)) {
                            auto int_val = pybind11::cast<int>(value);
                            json_str += std::to_string(int_val);
                        } else if (pybind11::isinstance<pybind11::bool_>(value)) {
                            auto bool_val = pybind11::cast<bool>(value);
                            json_str += bool_val ? "true" : "false";
                        }
                    }
                    json_str += "}";
                }
                json_str += "]";
                script_str = berialdraw::String(json_str.c_str());
            } else {
                throw std::invalid_argument("script must be a string or a list of dictionaries");
            }
            
            berialdraw::String snapshot_str = snapshot_name.is_none() ? berialdraw::String("") : py_to_string(snapshot_name);
            self.play_script(script_str, snapshot_str);
        }, pybind11::arg("script_input"), pybind11::arg("snapshot_name") = pybind11::none())
        .def("snapshot", [](berialdraw::Notifier& self, const pybind11::object& filename) {
            berialdraw::String filename_str = py_to_string(filename);
            self.snapshot(filename_str);
        }, pybind11::arg("filename"));
}