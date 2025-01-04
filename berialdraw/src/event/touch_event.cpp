#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
TouchEvent::TouchEvent(const Point & point, enum TouchState state) : 
	Event(TouchEvent::type_id()),
	m_position(point),
	m_state(state)
{
}

/** Destructor */
TouchEvent::~TouchEvent()
{
}

/** Return the event type */
enum TouchEvent::TouchState TouchEvent::state() const
{
	return m_state;
}

const Point & TouchEvent::position() const
{
	return m_position;
}

/** Get type */
uint32_t TouchEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(TouchEvent).hash_code();
	}
	return result;
}

/** To string */
void TouchEvent::to_string(String & str) const
{
	str.print("{'type':'touch','x':%3d,'y':%3d,'state':'%s'},",m_position.x(), m_position.y(), (m_state == TOUCH_UP ? "up" : (m_state == TOUCH_DOWN ? "down" : "move")));
}
