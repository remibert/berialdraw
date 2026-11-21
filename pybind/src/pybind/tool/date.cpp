#include "pybind/pyberialdraw.hpp"

void bind_date(py::module& m)
{
	py::class_<berialdraw::CalendarDate>(m, "CalendarDate")
		.def(py::init([](int year, int month, int day)
		{
			if (month < 1 || month > 255 || day < 1 || day > 255)
			{
				throw py::value_error("month and day must be positive values");
			}
			berialdraw::CalendarDate result;
			result.year = year;
			result.month = (uint8_t)month;
			result.day = (uint8_t)day;
			return result;
		}), py::arg("year"), py::arg("month"), py::arg("day"))
		.def_readwrite("year", &berialdraw::CalendarDate::year)
		.def_readwrite("month", &berialdraw::CalendarDate::month)
		.def_readwrite("day", &berialdraw::CalendarDate::day)
		.def("is_valid", &berialdraw::CalendarDate::is_valid);

	py::class_<berialdraw::Date>(m, "Date")
		.def_static("today", []() -> py::object
		{
			berialdraw::CalendarDate result;
			if (berialdraw::Date::today(result))
			{
				return py::cast(result);
			}
			return py::none();
		}, "Return the local date, or None when the system clock is unavailable")
		.def_static("days_in_month", [](int year, int month)
		{
			if (month < 1 || month > 12)
			{
				return (uint8_t)0;
			}
			return berialdraw::Date::days_in_month(year, (uint8_t)month);
		}, py::arg("year"), py::arg("month"))
		.def_static("weekday_monday", &berialdraw::Date::weekday_monday, py::arg("date"));
}
