#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
SlideEvent::SlideEvent(int32_t value, Widget * widget) : 
	Event(SlideEvent::type_id()),
	m_value(value),
	m_widget(widget)
{
}

/** Destructor */
SlideEvent::~SlideEvent()
{
}

/** Get type */
uint32_t SlideEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(SlideEvent).hash_code();
	}
	return result;
}

/** Return the value widget */
Widget * SlideEvent::widget() const
{
	return m_widget;
}

/** Return the value state */
int32_t SlideEvent::value() const
{
	return m_value;
}

/** To string */
void SlideEvent::to_string(String & str) const
{
	str.print("{'type':'slide','value':%d},",m_value);
}
