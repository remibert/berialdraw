#include "pybind/pyberialdraw.hpp"
void bind_uimanager(pybind11::module_& m) {
	pybind11::enum_<berialdraw::WindowMode>(m, "WindowMode")
		.value("FIXED", berialdraw::WindowMode::FIXED)
		.value("RESIZABLE", berialdraw::WindowMode::RESIZABLE)
		.export_values();

	pybind11::enum_<berialdraw::WindowState>(m, "WindowState")
		.value("NORMAL", berialdraw::WindowState::NORMAL)
		.value("MAXIMIZED", berialdraw::WindowState::MAXIMIZED)
		.value("FULLSCREEN", berialdraw::WindowState::FULLSCREEN)
		.export_values();

    pybind11::class_<berialdraw::UIManager>(m, "UIManager")
        .def_static("init", [](pybind11::object device, berialdraw::Dim width, berialdraw::Dim height,
                                berialdraw::Framebuf::Type type, uint32_t scale,
                                const berialdraw::String& root_dir, const std::string& title,
                                berialdraw::WindowMode window_mode, berialdraw::WindowState window_state) {
            if (device.is_none()) {
                device = pybind11::module_::import("pyberialdraw").attr("DeviceScreen")(title);
            }
            berialdraw::UIManager::init(device.cast<berialdraw::Device*>(), width, height, type, scale, root_dir, window_mode, window_state);
        }, pybind11::arg("device") = pybind11::none(),
           pybind11::arg("width") = 480,
           pybind11::arg("height") = 480,
           pybind11::arg("type") = berialdraw::Framebuf::ARGB8888,
           pybind11::arg("scale") = 1,
           pybind11::arg("root_dir") = berialdraw::String("./resources;../resources;zip://resources.zip/resources"),
           pybind11::arg("title") = "",
           pybind11::arg("window_mode") = berialdraw::WindowMode::FIXED,
           pybind11::arg("window_state") = berialdraw::WindowState::NORMAL)
        .def_static("deinit", &berialdraw::UIManager::deinit)
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
            
        .def_property_static("palette",
            // Getter - pas de méthode getter dans Colors, on retourne 0
            [](pybind11::object) -> uint32_t {
                return 0; // TODO: ajouter getter si disponible
            },
            // Setter
            [](pybind11::object, uint32_t color) {
                berialdraw::UIManager::colors()->palette(color);
            }, "Color palette")
            
        .def_property_static("clipboard",
            // Getter
            [](pybind11::object) -> pybind11::object {
                const berialdraw::String& text = berialdraw::UIManager::clipboard()->text();
                if (text.size() > 0) {
                    return pybind11::cast(std::string(text.c_str()));
                } else {
                    return pybind11::none();
                }
            },
            // Setter - accepte string ou None pour effacer
            [](pybind11::object, pybind11::object value) {
                if (value.is_none()) {
                    // Effacer le clipboard en mettant du texte vide
                    berialdraw::UIManager::clipboard()->text(berialdraw::String(""));
                } else {
                    // Copier le texte
                    std::string text = pybind11::cast<std::string>(value);
                    berialdraw::UIManager::clipboard()->text(berialdraw::String(text.c_str()));
                }
            }, "Clipboard content (string or None to clear)");
}
