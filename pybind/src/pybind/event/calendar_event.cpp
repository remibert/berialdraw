#include "pybind/pyberialdraw.hpp"

void bind_calendar_event(py::module& m)
{
	py::class_<berialdraw::CalendarEvent, berialdraw::Event>(m, "CalendarEvent")
		.def_property_readonly("date", [](const berialdraw::CalendarEvent& self)
		{
			return self.date();
		})
		.def_property_readonly("widget", &berialdraw::CalendarEvent::widget,
			py::return_value_policy::reference);
}
