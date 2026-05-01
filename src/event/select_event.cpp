#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
SelectEvent::SelectEvent(enum Status status, const Point & position, Widget * widget) : 
	Event(SelectEvent::type_id()),
	m_status(status),
	m_position(position),
	m_widget(widget)
{
}

/** Destructor */
SelectEvent::~SelectEvent()
{
}

/** Get type */
uint32_t SelectEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(SelectEvent).hash_code();
	}
	return result;
}

const Point & SelectEvent::position() const
{
	return m_position;
}

/** Return the widget */
Widget * SelectEvent::widget() const
{
	return m_widget;
}

/** Return the selection status */
enum SelectEvent::Status SelectEvent::status() const
{
	return m_status;
}


/** To string */
void SelectEvent::to_string(String & str) const
{
	str.print("{'type':'select','x':%d,'y':%d,'status':'%s'},",m_position.x(), m_position.y(), 
		(m_status == SelectEvent::SELECT_START ? "start" : 
		(m_status == SelectEvent::SELECT_END ? "end" : "continu")));
}
