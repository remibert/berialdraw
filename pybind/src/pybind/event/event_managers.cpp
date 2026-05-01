#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

// Static variable definition for EventSystemManager to avoid linkage errors
bool EventSystemManager::is_shutting_down = false;

// Explicit instantiations of bridge_callback for all event types
// This ensures all needed symbols are generated during compilation

template<> void bridge_callback<berialdraw::KeyEvent>(berialdraw::Widget* widget, const berialdraw::KeyEvent& event) {
	auto manager = EventSystemManager::instance().get_manager<berialdraw::KeyEvent>(widget);
	if (manager && !manager->empty()) {
		if (widget->focused())
		{
			manager->trigger(widget, event);
		}
	}
}


// Automatic cleanup function called by Python at shutdown
void automatic_cleanup() {
	// Mark the system as shutting down to prevent new operations
	EventSystemManager::instance().mark_shutdown();
}

void bind_event_managers(pybind11::module_& m) {
	// Note: CallbackManager classes are NOT exposed to Python.
	// They are managed internally and accessed through widget event properties.
	// This avoids the pybind11 template instantiation issue that causes
	// extremely long function signatures and memory corruption.
	
	// Automatically register cleanup on Python shutdown
	// Using Py_AtExit to ensure cleanup happens before Python shutdown
	if (Py_IsInitialized()) {
		if (Py_AtExit(automatic_cleanup) != 0) {
			// If registration fails, display a warning
			pybind11::print("Warning: Could not register automatic cleanup");
		}
	}
}

