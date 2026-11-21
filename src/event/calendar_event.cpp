#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create a calendar date changed event.
CalendarEvent::CalendarEvent(const CalendarDate & date, Widget * widget) :
	Event(CalendarEvent::type_id()),
	m_date(date),
	m_widget(widget)
{
}

// Destroy the calendar date changed event.
CalendarEvent::~CalendarEvent()
{
}

// Return the changed date.
const CalendarDate & CalendarEvent::date() const
{
	return m_date;
}

// Return the event source widget.
Widget * CalendarEvent::widget() const
{
	return m_widget;
}

// Return the event type identifier.
uint32_t CalendarEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(CalendarEvent).hash_code();
	}
	return result;
}

// Serialize the event for logging.
void CalendarEvent::to_string(String & str) const
{
	str.print("{'type':'calendar_date_changed','year':%d,'month':%u,'day':%u},",
		m_date.year, (unsigned int)m_date.month, (unsigned int)m_date.day);
}
