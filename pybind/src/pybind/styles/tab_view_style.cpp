#include "pybind/pyberialdraw.hpp"

void bind_tab_view_style(pybind11::module_& m)
{
	pybind11::enum_<berialdraw::TabPosition>(m, "TabPosition")
		.value("TOP", berialdraw::TabPosition::TOP)
		.value("RIGHT", berialdraw::TabPosition::RIGHT)
		.value("BOTTOM", berialdraw::TabPosition::BOTTOM)
		.value("LEFT", berialdraw::TabPosition::LEFT);

	pybind11::class_<berialdraw::TabViewStyle, berialdraw::Style>(m, "TabViewStyle")
		.def(pybind11::init<>())
		.def_property("tab_position",
			[](const berialdraw::TabViewStyle& self) { return self.tab_position(); },
			[](berialdraw::TabViewStyle& self, berialdraw::TabPosition value) { self.tab_position(value); })
		.def_property("tab_margin",
			[](const berialdraw::TabViewStyle& self) { return self.tab_margin(); },
			[](berialdraw::TabViewStyle& self, const berialdraw::Margin& value) { self.tab_margin(value); });
}