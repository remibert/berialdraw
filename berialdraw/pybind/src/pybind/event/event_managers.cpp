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
	// Bind CallbackManager classes for all event types
	// Each event type has its own CallbackManager with a unique name

	pybind11::class_<CallbackManager<berialdraw::ClickEvent>>(m, "ClickCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::ClickEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::ClickEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::ClickEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::ClickEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::KeyEvent>>(m, "KeyCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::KeyEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::KeyEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::KeyEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::KeyEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::CheckEvent>>(m, "CheckCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::CheckEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::CheckEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::CheckEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::CheckEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::SelectEvent>>(m, "SelectCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::SelectEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::SelectEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::SelectEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::SelectEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::SlideEvent>>(m, "SlideCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::SlideEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::SlideEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::SlideEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::SlideEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::ScrollEvent>>(m, "ScrollCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::ScrollEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::ScrollEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::ScrollEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::ScrollEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::FocusEvent>>(m, "FocusCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::FocusEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::FocusEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::FocusEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::FocusEvent>& self) { return !self.empty(); });

	pybind11::class_<CallbackManager<berialdraw::TouchEvent>>(m, "TouchCallbackManager")
		.def("__iadd__", &CallbackManager<berialdraw::TouchEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("add", &CallbackManager<berialdraw::TouchEvent>::add, pybind11::return_value_policy::reference_internal)
		.def("__len__", &CallbackManager<berialdraw::TouchEvent>::size)
		.def("__bool__", [](const CallbackManager<berialdraw::TouchEvent>& self) { return !self.empty(); });

	// Automatically register cleanup on Python shutdown
	// Using Py_AtExit to ensure cleanup happens before Python shutdown
	if (Py_IsInitialized()) {
		if (Py_AtExit(automatic_cleanup) != 0) {
			// If registration fails, display a warning
			pybind11::print("Warning: Could not register automatic cleanup");
		}
	}
}

