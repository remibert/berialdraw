#include "pybind/pyberialdraw.hpp"

void bind_tab_view(pybind11::module_& m)
{
	pybind11::class_<berialdraw::TabView, berialdraw::Widget,
		berialdraw::BorderStyle, berialdraw::TabViewStyle,
		std::unique_ptr<berialdraw::TabView, pybind11::nodelete>>(m, "TabView")
		.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal,
			pybind11::keep_alive<1, 2>())
		.def("add_tab", [](berialdraw::TabView& self, const std::string& title) {
			return self.add_tab(berialdraw::String(title.c_str()));
		}, pybind11::return_value_policy::reference_internal)
		.def("remove_tab", pybind11::overload_cast<berialdraw::Dim>(&berialdraw::TabView::remove_tab))
		.def("remove_tab", [](berialdraw::TabView& self, const std::string& title) {
			return self.remove_tab(berialdraw::String(title.c_str()));
		})
		.def("tab_count", &berialdraw::TabView::tab_count)
		.def("selected_index", &berialdraw::TabView::selected_index)
		.def("selected_page", &berialdraw::TabView::selected_page, pybind11::return_value_policy::reference_internal)
		.def("page", &berialdraw::TabView::page, pybind11::return_value_policy::reference_internal)
		.def("tab_button", &berialdraw::TabView::tab_button, pybind11::return_value_policy::reference_internal)
		.def("select", &berialdraw::TabView::select);
}