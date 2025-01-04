#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
CheckEvent::CheckEvent(bool checked, Widget * widget) : 
	Event(CheckEvent::type_id()),
	m_checked(checked),
	m_widget(widget)
{
}

/** Destructor */
CheckEvent::~CheckEvent()
{
}

/** Get type */
uint32_t CheckEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(CheckEvent).hash_code();
	}
	return result;
}

/** Return the checked widget */
Widget * CheckEvent::widget() const
{
	return m_widget;
}

/** Return the checked state */
bool CheckEvent::checked() const
{
	return m_checked;
}

/** To string */
void CheckEvent::to_string(String & str) const
{
	str.print("{'type':'check','checked':%s},",m_checked ? "true" : "false");
}
