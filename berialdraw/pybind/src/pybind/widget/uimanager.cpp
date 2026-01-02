#include "pybind/pyberialdraw.hpp"
void bind_uimanager(pybind11::module_& m) {
    pybind11::class_<berialdraw::UIManager>(m, "UIManager")
        .def_static("init", &berialdraw::UIManager::init, 
                    pybind11::arg("device"), 
                    pybind11::arg("width"), 
                    pybind11::arg("height"), 
                    pybind11::arg("type"), 
                    pybind11::arg("scale") = 1, 
                    pybind11::arg("root_dir") = berialdraw::String(""))
        .def_static("deinit", &berialdraw::UIManager::deinit)
        // .def_static("exporter", static_cast<void (*)(berialdraw::Exporter*)>(&berialdraw::UIManager::exporter))
        // .def_static("exporter", static_cast<berialdraw::Exporter* (*)()>(&berialdraw::UIManager::exporter), pybind11::return_value_policy::reference_internal)
        .def_static("framebuf", &berialdraw::UIManager::framebuf, pybind11::return_value_policy::reference)
        .def_static("notifier", &berialdraw::UIManager::notifier, pybind11::return_value_policy::reference)
        .def_static("device", &berialdraw::UIManager::device, pybind11::return_value_policy::reference)
        .def_static("fonts", &berialdraw::UIManager::fonts, pybind11::return_value_policy::reference)
        .def_static("invalidator", &berialdraw::UIManager::invalidator, pybind11::return_value_policy::reference)
        .def_static("styles", &berialdraw::UIManager::styles, pybind11::return_value_policy::reference)
        .def_static("colors", &berialdraw::UIManager::colors, pybind11::return_value_policy::reference)
        .def_static("desktop", &berialdraw::UIManager::desktop, pybind11::return_value_policy::reference)
        .def_static("settings", &berialdraw::UIManager::settings, pybind11::return_value_policy::reference)
        .def_static("is_initialized", &berialdraw::UIManager::is_initialized)
        
        // Propriétés avec getters/setters pour une API plus Pythonique
        .def_property_static("style", 
            // Getter
            [](pybind11::object) -> std::string {
                return std::string(berialdraw::UIManager::styles()->style().c_str());
            },
            // Setter  
            [](pybind11::object, const std::string& name) {
                berialdraw::UIManager::styles()->style(berialdraw::String(name.c_str()));
            }, "UI style name")
            
        .def_property_static("appearance",
            // Getter - pas de méthode getter dans Colors, on retourne une chaîne vide
            [](pybind11::object) -> std::string {
                return ""; // TODO: ajouter getter si disponible
            },
            // Setter
            [](pybind11::object, const std::string& name) {
                berialdraw::UIManager::colors()->appearance(name.c_str());
            }, "Appearance mode (light/dark)")
            
        .def_property_static("theme",
            // Getter - pas de méthode getter dans Colors, on retourne 0
            [](pybind11::object) -> uint32_t {
                return 0; // TODO: ajouter getter si disponible
            },
            // Setter
            [](pybind11::object, uint32_t color) {
                berialdraw::UIManager::colors()->theme(color);
            }, "Color theme");
}
