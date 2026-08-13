#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Timer event creation and identification
TimerEvent::TimerEvent(uint32_t timer_id, uint32_t elapsed_ticks, Widget* widget)
	: Event(TimerEvent::type_id())
	, m_timer_id(timer_id)
	, m_elapsed_ticks(elapsed_ticks)
	, m_widget(widget)
{
}

// Timer event destruction
TimerEvent::~TimerEvent()
{
}

// Get the timer identifier
uint32_t TimerEvent::timer_id() const
{
	return m_timer_id;
}

// Get the number of elapsed timer periods
uint32_t TimerEvent::elapsed_ticks() const
{
	return m_elapsed_ticks;
}

// Convert timer event to string for debugging
void TimerEvent::to_string(String & str) const
{
	str.print("{'type':'timer','id':%u,'ticks':%u}", m_timer_id, m_elapsed_ticks);
}

// Get unique type identifier for timer events
uint32_t TimerEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(TimerEvent).hash_code();
	}
	return result;
}
