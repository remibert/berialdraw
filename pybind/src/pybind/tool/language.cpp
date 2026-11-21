#include "pybind/pyberialdraw.hpp"

void bind_language(py::module& m)
{
	py::class_<berialdraw::Language>(m, "Language")
		.def("select", [](berialdraw::Language& self, const std::string& locale)
		{
			return self.select(locale.c_str());
		}, py::arg("locale"),
			"Select a language catalog by locale")
		.def_property_readonly("locale", [](const berialdraw::Language& self)
		{
			return std::string(self.locale().c_str());
		}, "Currently selected locale")
		.def("translate", [](const berialdraw::Language& self, const std::string& id)
		{
			berialdraw::String translated = self.translate(id.c_str());
			return std::string(translated.c_str());
		}, py::arg("id"),
			"Translate a message identifier");
}
