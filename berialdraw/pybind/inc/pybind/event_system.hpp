#pragma once
#include <pybind11/pybind11.h>
#include <map>
#include <memory>
#include <vector>
#include <typeindex>

// Generic callback manager for any event type
template<typename EventType>
class CallbackManager {
private:
    std::vector<pybind11::function> callbacks;
    
public:
    // Simple destructor - does nothing to avoid crashes
    ~CallbackManager() {
        // Automatic cleanup via Py_AtExit handles everything
    }
    
    // Assign a single callback
    void assign(const pybind11::function& callback) {
        callbacks.clear();
        callbacks.push_back(callback);
    }
    
    // Assign a list of callbacks
    void assign_list(const pybind11::list& callback_list) {
        callbacks.clear();
        for (auto item : callback_list) {
            callbacks.push_back(item.cast<pybind11::function>());
        }
    }
    
    // Add a callback (operator +=)
    CallbackManager& add(const pybind11::function& callback) {
        callbacks.push_back(callback);
        return *this;
    }
    
    // Call all callbacks
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

// Centralized system to manage callbacks for all widgets and events
class EventSystemManager {
private:
    // Map [Widget*][TypeIndex] -> CallbackManager* (raw pointers to avoid shared_ptr crashes)
    std::map<berialdraw::Widget*, std::map<std::type_index, void*>> widget_callbacks;
    static bool is_shutting_down;
    
public:
    // Destructor that does absolutely nothing
    ~EventSystemManager() {
        // DO NOTHING - leave raw pointers in place
        // to avoid any crash during shutdown
    }
    
    // Mark the system as shutting down
    static void mark_shutdown() {
        is_shutting_down = true;
    }
    
    // Get the callback manager for a widget and an event type
    template<typename EventType>
    CallbackManager<EventType>* get_manager(berialdraw::Widget* widget) {
        if (is_shutting_down) {
            return nullptr;
        }
        
        auto type_idx = std::type_index(typeid(EventType));
        
        if (widget_callbacks[widget].find(type_idx) == widget_callbacks[widget].end()) {
            // Create with new and store the raw pointer
            widget_callbacks[widget][type_idx] = new CallbackManager<EventType>();
        }
        
        return static_cast<CallbackManager<EventType>*>(widget_callbacks[widget][type_idx]);
    }
    
    // Clean up callbacks for a widget (does nothing during shutdown)
    void cleanup_widget(berialdraw::Widget* widget) {
        if (!is_shutting_down) {
            // Optional: delete managers for this widget
            // But we can also leave them as they will be cleaned up globally
            widget_callbacks.erase(widget);
        }
    }
    
    // Simple singleton instance
    static EventSystemManager& instance() {
        static EventSystemManager manager;
        return manager;
    }
};

// Generic bridge functions for each event type
template<typename EventType>
void bridge_callback(berialdraw::Widget* widget, const EventType& event) {
    auto manager = EventSystemManager::instance().get_manager<EventType>(widget);
    if (manager && !manager->empty()) {
        manager->trigger(widget, event);
    }
}

// Declaration of specialization for KeyEvent (defined in event_managers.cpp)
template<>
void bridge_callback<berialdraw::KeyEvent>(berialdraw::Widget* widget, const berialdraw::KeyEvent& event);

// Macros to simplify creation of event properties
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