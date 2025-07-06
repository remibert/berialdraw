#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
FocusEvent::FocusEvent(bool focused, Widget * widget) : 
	Event(FocusEvent::type_id()),
	m_focused(focused),
	m_widget(widget)
{
}

/** Destructor */
FocusEvent::~FocusEvent()
{
}

/** Get type */
uint32_t FocusEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(FocusEvent).hash_code();
	}
	return result;
}

/** Return the focused widget */
Widget * FocusEvent::widget() const
{
	return m_widget;
}

/** Return the focused state */
bool FocusEvent::focused() const
{
	return m_focused;
}

/** To string */
void FocusEvent::to_string(String & str) const
{
	str.print("{'type':'focus','focused':%s},",m_focused ? "true" : "false");
}
