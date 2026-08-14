#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_list(pybind11::module_& m) {
	pybind11::class_<berialdraw::List, berialdraw::ScrollableContent, berialdraw::ListStyle>(m, "List")
		.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
		
		// Create new list item
		.def("new_item", &berialdraw::List::new_item, pybind11::return_value_policy::reference_internal, PYBIND11_RELEASE_GIL,
			"Create new list item")
		
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
