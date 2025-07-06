#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ClickEvent::ClickEvent(const Point & position, Widget * clicked) : 
	Event(ClickEvent::type_id()),
	m_position(position),
	m_widget(clicked)
{
}

/** Destructor */
ClickEvent::~ClickEvent()
{
}

/** Get type */
uint32_t ClickEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(ClickEvent).hash_code();
	}
	return result;
}

const Point & ClickEvent::position() const
{
	return m_position;
}

/** Return the widget */
Widget * ClickEvent::widget() const 
{
	return m_widget;
}

/** To string */
void ClickEvent::to_string(String & str) const
{
	str.print("{'type':'click','x':%d,'y':%d},",m_position.x(), m_position.y());
}
