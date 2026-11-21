#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create the calendar style.
CalendarStyle::CalendarStyle()
{
}

// Serialize calendar style properties.
void CalendarStyle::serialize(JsonIterator & it)
{
	it[StyleNames::CALENDAR_WEEKDAY_TEXT_COLOR] = m_weekday_text_color;
	m_weekday_font_size.serialize(StyleNames::CALENDAR_WEEKDAY_FONT_SIZE, it);
	it[StyleNames::CALENDAR_WEEKDAY_BACK_COLOR] = m_weekday_back_color;
	it[StyleNames::CALENDAR_OUTSIDE_DAY_TEXT_COLOR] = m_outside_day_text_color;
	it[StyleNames::CALENDAR_OUTSIDE_DAY_BACK_COLOR] = m_outside_day_back_color;
	it[StyleNames::CALENDAR_DAY_TEXT_COLOR] = m_day_text_color;
	m_day_font_size.serialize(StyleNames::CALENDAR_DAY_FONT_SIZE, it);
	it[StyleNames::CALENDAR_DAY_BACK_COLOR] = m_day_back_color;
	it[StyleNames::CALENDAR_DAY_SELECTED_TEXT_COLOR] = m_day_selected_text_color;
	it[StyleNames::CALENDAR_DAY_SELECTED_BACK_COLOR] = m_day_selected_back_color;
	it[StyleNames::GRIDSTYLE_GRID_COLOR] = m_grid_color;
	it[StyleNames::CALENDAR_GRID_THICKNESS] = (unsigned int)m_grid_thickness;
	it[StyleNames::CALENDAR_DAY_BORDER_COLOR] = m_day_border_color;
	it[StyleNames::CALENDAR_DAY_BORDER_THICKNESS] = (unsigned int)m_day_border_thickness;
}

// Unserialize calendar style properties.
void CalendarStyle::unserialize(JsonIterator & it)
{
	m_weekday_text_color = (uint32_t)(int)(it[StyleNames::CALENDAR_WEEKDAY_TEXT_COLOR] | (int)m_weekday_text_color);
	m_weekday_font_size.unserialize(StyleNames::CALENDAR_WEEKDAY_FONT_SIZE, it);
	m_weekday_back_color = (uint32_t)(int)(it[StyleNames::CALENDAR_WEEKDAY_BACK_COLOR] | (int)m_weekday_back_color);
	m_outside_day_text_color = (uint32_t)(int)(it[StyleNames::CALENDAR_OUTSIDE_DAY_TEXT_COLOR] | (int)m_outside_day_text_color);
	m_outside_day_back_color = (uint32_t)(int)(it[StyleNames::CALENDAR_OUTSIDE_DAY_BACK_COLOR] | (int)m_outside_day_back_color);
	m_day_text_color = (uint32_t)(int)(it[StyleNames::CALENDAR_DAY_TEXT_COLOR] | (int)m_day_text_color);
	m_day_font_size.unserialize(StyleNames::CALENDAR_DAY_FONT_SIZE, it);
	m_day_back_color = (uint32_t)(int)(it[StyleNames::CALENDAR_DAY_BACK_COLOR] | (int)m_day_back_color);
	m_day_selected_text_color = (uint32_t)(int)(it[StyleNames::CALENDAR_DAY_SELECTED_TEXT_COLOR] | (int)m_day_selected_text_color);
	m_day_selected_back_color = (uint32_t)(int)(it[StyleNames::CALENDAR_DAY_SELECTED_BACK_COLOR] | (int)m_day_selected_back_color);
	m_grid_color = (uint32_t)(int)(it[StyleNames::GRIDSTYLE_GRID_COLOR] | (int)m_grid_color);
	m_grid_thickness = (Dim)(it[StyleNames::CALENDAR_GRID_THICKNESS] | (unsigned int)m_grid_thickness);
	m_day_border_color = (uint32_t)(int)(it[StyleNames::CALENDAR_DAY_BORDER_COLOR] | (int)m_day_border_color);
	m_day_border_thickness = (Dim)(it[StyleNames::CALENDAR_DAY_BORDER_THICKNESS] | (unsigned int)m_day_border_thickness);
}

// Copy calendar style properties.
CalendarStyle & CalendarStyle::operator=(const CalendarStyle & other)
{
	set(other);
	return *this;
}

// Set calendar style properties.
void CalendarStyle::set(const CalendarStyle & other)
{
	if (this != &other)
	{
		m_weekday_text_color = other.m_weekday_text_color;
		m_weekday_font_size = other.m_weekday_font_size;
		m_weekday_back_color = other.m_weekday_back_color;
		m_outside_day_text_color = other.m_outside_day_text_color;
		m_outside_day_back_color = other.m_outside_day_back_color;
		m_day_text_color = other.m_day_text_color;
		m_day_font_size = other.m_day_font_size;
		m_day_back_color = other.m_day_back_color;
		m_day_selected_text_color = other.m_day_selected_text_color;
		m_day_selected_back_color = other.m_day_selected_back_color;
		m_grid_color = other.m_grid_color;
		m_grid_thickness = other.m_grid_thickness;
		m_day_border_color = other.m_day_border_color;
		m_day_border_thickness = other.m_day_border_thickness;
	}
}

// Create a calendar style.
Style * CalendarStyle::create()
{
	return new CalendarStyle;
}
