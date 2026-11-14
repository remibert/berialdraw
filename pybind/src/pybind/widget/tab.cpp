#include "pybind/pyberialdraw.hpp"

void bind_tab(pybind11::module_& m)
{
	pybind11::class_<berialdraw::Tab, berialdraw::Button,
		std::unique_ptr<berialdraw::Tab, pybind11::nodelete>>(m, "Tab")
		.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal,
			pybind11::keep_alive<1, 2>())
		.def_property("position",
			[](const berialdraw::Tab& self) { return self.position(); },
			[](berialdraw::Tab& self, berialdraw::TabPosition value) { self.position(value); });
}