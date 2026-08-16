#include "pybind/pyberialdraw.hpp"

void bind_list_item_style(pybind11::module_& m) {
	pybind11::class_<berialdraw::ListItemStyle, berialdraw::Style> cls(m, "ListItemStyle");
	cls.def(pybind11::init<>());

	// leading property
	cls.def_property("leading",
		[](berialdraw::ListItemStyle& self) -> std::string { return std::string(self.leading().c_str()); },
		[](berialdraw::ListItemStyle& self, const std::string& value) { self.leading(value.c_str()); },
		"Leading text");

	// trailing property
	cls.def_property("trailing",
		[](berialdraw::ListItemStyle& self) -> std::string { return std::string(self.trailing().c_str()); },
		[](berialdraw::ListItemStyle& self, const std::string& value) { self.trailing(value.c_str()); },
		"Trailing text");

	// Serialization
	cls.def("serialize", &berialdraw::ListItemStyle::serialize, pybind11::arg("it"), PYBIND11_RELEASE_GIL);
	cls.def("unserialize", &berialdraw::ListItemStyle::unserialize, pybind11::arg("it"), PYBIND11_RELEASE_GIL);
}
