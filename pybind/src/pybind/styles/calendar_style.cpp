#include "pybind/pyberialdraw.hpp"

void bind_calendar_style(py::module_& m)
{
	py::class_<berialdraw::CalendarStyle, berialdraw::Style>(m, "CalendarStyle")
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_WEEKDAY_TEXT_COLOR, &berialdraw::CalendarStyle::weekday_text_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_WEEKDAY_FONT_SIZE, &berialdraw::CalendarStyle::weekday_font_size)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_WEEKDAY_BACK_COLOR, &berialdraw::CalendarStyle::weekday_back_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_OUTSIDE_DAY_TEXT_COLOR, &berialdraw::CalendarStyle::outside_day_text_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_OUTSIDE_DAY_BACK_COLOR, &berialdraw::CalendarStyle::outside_day_back_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_TEXT_COLOR, &berialdraw::CalendarStyle::day_text_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_FONT_SIZE, &berialdraw::CalendarStyle::day_font_size)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_BACK_COLOR, &berialdraw::CalendarStyle::day_back_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_SELECTED_TEXT_COLOR, &berialdraw::CalendarStyle::day_selected_text_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_SELECTED_BACK_COLOR, &berialdraw::CalendarStyle::day_selected_back_color)
		.def_property_readonly(berialdraw::StyleNames::GRIDSTYLE_GRID_COLOR, &berialdraw::CalendarStyle::grid_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_GRID_THICKNESS, &berialdraw::CalendarStyle::grid_thickness)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_BORDER_COLOR, &berialdraw::CalendarStyle::day_border_color)
		.def_property_readonly(berialdraw::StyleNames::CALENDAR_DAY_BORDER_THICKNESS, &berialdraw::CalendarStyle::day_border_thickness);
}
