#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_list_item(pybind11::module_& m) {
	pybind11::class_<berialdraw::ListItem, berialdraw::Widget, berialdraw::TextStyle, berialdraw::BorderStyle, berialdraw::ListItemStyle>(m, "ListItem")
		.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
		
		// ListItemStyle properties (leading, trailing)
		.def_property("leading",
			[](berialdraw::ListItem& self) -> std::string { return std::string(self.leading().c_str()); },
			[](berialdraw::ListItem& self, const std::string& value) { self.leading(value.c_str()); },
			"Leading text")
		.def_property("trailing",
			[](berialdraw::ListItem& self) -> std::string { return std::string(self.trailing().c_str()); },
			[](berialdraw::ListItem& self, const std::string& value) { self.trailing(value.c_str()); },
			"Trailing text")
		
		// Serialization
		.def("serialize", &berialdraw::ListItem::serialize, pybind11::arg("it"), PYBIND11_RELEASE_GIL)
		.def("unserialize", &berialdraw::ListItem::unserialize, pybind11::arg("it"), PYBIND11_RELEASE_GIL)
		
		// Copy methods
		.def("copy", static_cast<void (berialdraw::ListItem::*)(const berialdraw::ListItem&)>(&berialdraw::ListItem::copy), pybind11::arg("list_item"), PYBIND11_RELEASE_GIL)
		.def("copy", static_cast<void (berialdraw::ListItem::*)(const berialdraw::ListItem*)>(&berialdraw::ListItem::copy), pybind11::arg("list_item"), PYBIND11_RELEASE_GIL)
		
		// Event system integration
		BIND_EVENT_PROPERTY(berialdraw::ListItem, berialdraw::ClickEvent, on_click)
		BIND_EVENT_PROPERTY(berialdraw::ListItem, berialdraw::TouchEvent, on_touch)
		BIND_EVENT_PROPERTY(berialdraw::ListItem, berialdraw::FocusEvent, on_focus);
}
