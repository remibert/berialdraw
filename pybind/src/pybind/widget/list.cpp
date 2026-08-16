#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_list(pybind11::module_& m) {
	// Helper lambda to handle string, list, and callable arguments
	// This avoids code duplication across append/prepend/insert methods
	auto handle_list_operation = [](pybind11::object arg, auto on_string, auto on_list, auto on_callable) -> pybind11::object {
		if (pybind11::isinstance<pybind11::str>(arg))
		{
			// Single string: return result
			return on_string(pybind11::cast<std::string>(arg).c_str());
		}
		else if (pybind11::isinstance<pybind11::list>(arg))
		{
			// List of strings: iterate and apply on_list for each
			auto py_list = pybind11::cast<pybind11::list>(arg);
			for (auto item : py_list)
			{
				on_list(pybind11::cast<std::string>(item).c_str());
			}
			return pybind11::none();
		}
		else if (pybind11::isinstance<pybind11::function>(arg))
		{
			// Callable: wrap Python function and ignore return value
			// This allows Python functions to return tuples, None, or any other value
			auto py_func = pybind11::cast<pybind11::object>(arg);
			auto config = [py_func](berialdraw::ListItem* item) {
				pybind11::gil_scoped_acquire acquire;
				py_func(item);  // Call Python function, ignore return value
			};
			return on_callable(config);
		}
		throw pybind11::type_error("Argument must be a string, list of strings, or callable");
	};

	pybind11::class_<berialdraw::List, berialdraw::ScrollableContent>(m, "List")
		.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
		
		// Create new list item
		.def("new_item", &berialdraw::List::new_item, pybind11::return_value_policy::reference_internal, PYBIND11_RELEASE_GIL,
			"Create new list item")
		
		// Append method (handles string, list of strings, and callable)
		.def("append", [handle_list_operation](berialdraw::List& self, pybind11::object arg) -> pybind11::object {
			return handle_list_operation(arg,
				[&self](const char* text) { return pybind11::cast(self.append(text)); },
				[&self](const char* text) { self.append(text); },
				[&self](const auto& config) { return pybind11::cast(self.append(config)); }
			);
		}, pybind11::arg("arg"),
			"Append item(s): string, list of strings, or callable configuration")
		
		// Prepend method (handles string, list of strings, and callable)
		.def("prepend", [handle_list_operation](berialdraw::List& self, pybind11::object arg) -> pybind11::object {
			return handle_list_operation(arg,
				[&self](const char* text) { return pybind11::cast(self.prepend(text)); },
				[&self](const char* text) { self.insert((int)self.count(), text); },
				[&self](const auto& config) { return pybind11::cast(self.prepend(config)); }
			);
		}, pybind11::arg("arg"),
			"Prepend item(s): string, list of strings, or callable configuration")
		
		// Insert method (handles string, list of strings, and callable at specific index)
		.def("insert", [handle_list_operation](berialdraw::List& self, int index, pybind11::object arg) -> pybind11::object {
			return handle_list_operation(arg,
				[&self, index](const char* text) { return pybind11::cast(self.insert(index, text)); },
				[&self, index](const char* text) { self.insert(index, text); },
				[&self, index](const auto& config) { return pybind11::cast(self.insert(index, config)); }
			);
		}, pybind11::arg("index"), pybind11::arg("arg"),
			"Insert item(s) at index: string, list of strings, or callable configuration")
		
		// Item access
		.def("at", &berialdraw::List::at, pybind11::return_value_policy::reference_internal, pybind11::arg("index"), PYBIND11_RELEASE_GIL,
			"Get item at index (Python-style negative indexing)")
		.def("__getitem__", &berialdraw::List::operator[], pybind11::return_value_policy::reference_internal, pybind11::arg("index"), PYBIND11_RELEASE_GIL,
			"Array access operator for list items")
		
		// List operations
		.def("count", &berialdraw::List::count, PYBIND11_RELEASE_GIL,
			"Get number of items in the list")
		.def("is_empty", &berialdraw::List::is_empty, PYBIND11_RELEASE_GIL,
			"Check if list is empty")
		.def("remove", &berialdraw::List::remove, pybind11::arg("index"), PYBIND11_RELEASE_GIL,
			"Remove item at index")
		.def("clear", &berialdraw::List::clear, PYBIND11_RELEASE_GIL,
			"Remove all items from the list")
		
		// Serialization
		.def("serialize", &berialdraw::List::serialize, pybind11::arg("it"), PYBIND11_RELEASE_GIL)
		.def("unserialize", &berialdraw::List::unserialize, pybind11::arg("it"), PYBIND11_RELEASE_GIL)
		
		// Copy methods
		.def("copy", static_cast<void (berialdraw::List::*)(const berialdraw::List&)>(&berialdraw::List::copy), pybind11::arg("list"), PYBIND11_RELEASE_GIL)
		.def("copy", static_cast<void (berialdraw::List::*)(const berialdraw::List*)>(&berialdraw::List::copy), pybind11::arg("list"), PYBIND11_RELEASE_GIL)
		
		// Event system integration
		BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::ClickEvent, on_click)
		BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::TouchEvent, on_touch)
		BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::ScrollEvent, on_scroll);
}
