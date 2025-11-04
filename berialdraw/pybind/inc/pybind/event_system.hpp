#pragma once
#include <pybind11/pybind11.h>
#include <map>
#include <memory>
#include <vector>
#include <typeindex>

// Gestionnaire générique de callbacks pour n'importe quel type d'événement
template<typename EventType>
class CallbackManager {
private:
    std::vector<pybind11::function> callbacks;
    
public:
    // Destructeur simple - ne fait rien pour éviter les crashes
    ~CallbackManager() {
        // Le nettoyage automatique via Py_AtExit s'occupe de tout
    }
    
    // Assignation d'une callback simple
    void assign(const pybind11::function& callback) {
        callbacks.clear();
        callbacks.push_back(callback);
    }
    
    // Assignation d'une liste de callbacks
    void assign_list(const pybind11::list& callback_list) {
        callbacks.clear();
        for (auto item : callback_list) {
            callbacks.push_back(item.cast<pybind11::function>());
        }
    }
    
    // Ajouter une callback (opérateur +=)
    CallbackManager& add(const pybind11::function& callback) {
        callbacks.push_back(callback);
        return *this;
    }
    
    // Appeler toutes les callbacks
    void trigger(berialdraw::Widget* widget, const EventType& event) {
        if (!Py_IsInitialized()) return;
        
        for (auto& callback : callbacks) {
            if (callback) {
                try {
                    callback(widget, event);
                } catch (const std::exception& e) {
                    pybind11::print("Callback error:", e.what());
                }
            }
        }
    }
    
    bool empty() const { return callbacks.empty(); }
    size_t size() const { return callbacks.size(); }
};

// Système centralisé pour gérer les callbacks de tous widgets et événements
class EventSystemManager {
private:
    // Map [Widget*][TypeIndex] -> CallbackManager* (pointeurs bruts pour éviter les crashes de shared_ptr)
    std::map<berialdraw::Widget*, std::map<std::type_index, void*>> widget_callbacks;
    static bool is_shutting_down;
    
public:
    // Destructor qui ne fait absolument rien
    ~EventSystemManager() {
        // NE RIEN FAIRE - laisse les pointeurs bruts en place
        // pour éviter tout crash lors du shutdown
    }
    
    // Marquer le système comme en cours de shutdown
    static void mark_shutdown() {
        is_shutting_down = true;
    }
    
    // Obtenir le gestionnaire de callbacks pour un widget et un type d'événement
    template<typename EventType>
    CallbackManager<EventType>* get_manager(berialdraw::Widget* widget) {
        if (is_shutting_down) {
            return nullptr;
        }
        
        auto type_idx = std::type_index(typeid(EventType));
        
        if (widget_callbacks[widget].find(type_idx) == widget_callbacks[widget].end()) {
            // Créer avec new et stocker le pointeur brut
            widget_callbacks[widget][type_idx] = new CallbackManager<EventType>();
        }
        
        return static_cast<CallbackManager<EventType>*>(widget_callbacks[widget][type_idx]);
    }
    
    // Nettoyer les callbacks d'un widget (ne fait rien lors du shutdown)
    void cleanup_widget(berialdraw::Widget* widget) {
        if (!is_shutting_down) {
            // Optionnel: supprimer les managers de ce widget
            // Mais on peut aussi les laisser car ça sera nettoyé globalement
            widget_callbacks.erase(widget);
        }
    }
    
    // Instance singleton simple
    static EventSystemManager& instance() {
        static EventSystemManager manager;
        return manager;
    }
};

// Fonctions de pont génériques pour chaque type d'événement
template<typename EventType>
void bridge_callback(berialdraw::Widget* widget, const EventType& event) {
    auto manager = EventSystemManager::instance().get_manager<EventType>(widget);
    if (manager && !manager->empty()) {
        manager->trigger(widget, event);
    }
}

// Déclaration de la spécialisation pour KeyEvent (définie dans event_managers.cpp)
template<>
void bridge_callback<berialdraw::KeyEvent>(berialdraw::Widget* widget, const berialdraw::KeyEvent& event);

// Macros pour simplifier la création de propriétés d'événements
#define BIND_EVENT_PROPERTY(WidgetClass, EventType, property_name) \
    .def_property(#property_name, \
        [](WidgetClass& self) -> CallbackManager<EventType>* { \
            return EventSystemManager::instance().get_manager<EventType>(&self); \
        }, \
        [](WidgetClass& self, const pybind11::object& value) { \
            auto manager = EventSystemManager::instance().get_manager<EventType>(&self); \
            if (!manager) return; \
            \
            if (pybind11::isinstance<pybind11::function>(value)) { \
                manager->assign(value.cast<pybind11::function>()); \
            } else if (pybind11::isinstance<pybind11::list>(value)) { \
                manager->assign_list(value.cast<pybind11::list>()); \
            } else { \
                throw std::invalid_argument(#property_name " must be a function or list of functions"); \
            } \
            \
            self.template bind<EventType>(bridge_callback<EventType>); \
        }, \
        pybind11::return_value_policy::reference_internal)