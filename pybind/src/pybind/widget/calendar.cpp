#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_calendar(py::module_& m)
{
	py::class_<berialdraw::Calendar, berialdraw::Widget, berialdraw::PaddingStyle,
		berialdraw::BorderStyle, berialdraw::CalendarStyle,
		std::unique_ptr<berialdraw::Calendar, py::nodelete>>(m, "Calendar")
		.def(py::init<berialdraw::Widget*>(),
			py::arg("parent") = nullptr,
			py::return_value_policy::reference_internal,
			py::keep_alive<1, 2>())
		.def(py::init<berialdraw::Widget*, const berialdraw::CalendarDate&>(),
			py::arg("parent"), py::arg("date"),
			py::return_value_policy::reference_internal,
			py::keep_alive<1, 2>())
		.def_property("date",
			[](const berialdraw::Calendar& self)
			{
				return self.date();
			},
			[](berialdraw::Calendar& self, const berialdraw::CalendarDate& value)
			{
				if (!self.date(value))
				{
					throw py::value_error("invalid calendar date");
				}
			})
		.def_property("year",
			[](const berialdraw::Calendar& self)
			{
				return self.year();
			},
			[](berialdraw::Calendar& self, int value)
			{
				if (!self.year(value))
				{
					throw py::value_error("invalid calendar year");
				}
			})
		.def_property("month",
			[](const berialdraw::Calendar& self)
			{
				return self.month();
			},
			[](berialdraw::Calendar& self, int value)
			{
				if (value < 1 || value > 12 || !self.month((uint8_t)value))
				{
					throw py::value_error("invalid calendar month");
				}
			})
		.def_property("day",
			[](const berialdraw::Calendar& self)
			{
				return self.day();
			},
			[](berialdraw::Calendar& self, int value)
			{
				if (value < 1 || value > 31 || !self.day((uint8_t)value))
				{
					throw py::value_error("invalid calendar day");
				}
			})
		.def("previous_month", &berialdraw::Calendar::previous_month)
		.def("next_month", &berialdraw::Calendar::next_month)
		.def("today", &berialdraw::Calendar::today)
		BIND_EVENT_PROPERTY(berialdraw::Calendar, berialdraw::CalendarEvent, on_date_changed);
}
