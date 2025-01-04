#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ScrollEvent::ScrollEvent(const Point & shift, ScrollView * scroll_view) : 
	Event(ScrollEvent::type_id()),
	m_shift(shift),
	m_scroll_view(scroll_view)
{
}

/** Destructor */
ScrollEvent::~ScrollEvent()
{
}

const Point & ScrollEvent::shift() const
{
	return m_shift;
}

/** Return the scroll view widget */
ScrollView * ScrollEvent::scroll_view() const
{
	return m_scroll_view;
}

/** Get type */
uint32_t ScrollEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(ScrollEvent).hash_code();
	}
	return result;
}

/** To string */
void ScrollEvent::to_string(String & str) const
{
	str.print("{'type':'scroll','x':%d,'y':%d},",m_shift.x(), m_shift.y());
}
