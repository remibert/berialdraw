#include "berialdraw_imp.hpp"
#include "device/timer_manager_base.hpp"

using namespace berialdraw;

// Get next unique timer ID
uint32_t TimerManagerBase::get_next_timer_id()
{
	uint32_t timer_id = m_next_timer_id++;
	if (m_next_timer_id == 0)
	{
		m_next_timer_id = 1;  // Skip 0 to indicate invalid
	}
	return timer_id;
}

// Find timer record by ID
TimerManagerBase::TimerRecord* TimerManagerBase::find_timer(uint32_t timer_id)
{
	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		if (m_timers[i].id == timer_id)
		{
			return &m_timers[i];
		}
	}
	return nullptr;
}

// Find timer index by ID
int TimerManagerBase::find_timer_index(uint32_t timer_id)
{
	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		if (m_timers[i].id == timer_id)
		{
			return i;
		}
	}
	return -1;
}

// Cancel a timer
bool TimerManagerBase::cancel(uint32_t timer_id)
{
	int index = find_timer_index(timer_id);
	if (index >= 0)
	{
		m_timers.remove(index);
		return true;
	}
	return false;
}

// Stop all timers
uint32_t TimerManagerBase::stop_all()
{
	uint32_t count = (uint32_t)m_timers.size();
	m_timers.clear();
	return count;
}
