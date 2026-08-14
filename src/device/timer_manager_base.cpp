#include "berialdraw_imp.hpp"
#include "device/timer_manager_base.hpp"
#include "device/system_time.hpp"

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

// Schedule a new timer
uint32_t TimerManagerBase::schedule(uint32_t delay_ms, bool recurring, Widget* widget)
{
	if (delay_ms == 0)
	{
		return 0;  // Invalid interval
	}

	uint32_t timer_id = get_next_timer_id();

	TimerRecord record;
	record.id = timer_id;
	record.delay_ms = delay_ms;
	record.recurring = recurring;
	record.deadline_ms = SystemTime::get_time_ms() + delay_ms;
	record.last_fire_ms = 0;
	record.widget = widget;

	m_timers.push_back(record);
	return timer_id;
}

// Check for expired timers and generate events
void TimerManagerBase::check_timers()
{
	uint64_t now = SystemTime::get_time_ms();
	Vector<uint32_t> timers_to_remove;

	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		if (now >= m_timers[i].deadline_ms)
		{
			// Calculate how many periods have elapsed
			uint64_t missed_ms = now - m_timers[i].deadline_ms;
			uint32_t missed_ticks = (uint32_t)(missed_ms / m_timers[i].delay_ms);
			uint32_t elapsed_ticks = 1 + missed_ticks;  // 1 (current) + missed

			// Push timer event
			push_timer_event(m_timers[i].id, elapsed_ticks, m_timers[i].widget);

			if (m_timers[i].recurring)
			{
				// Recalibrate to avoid drift - set new deadline from now
				m_timers[i].deadline_ms += (uint64_t)m_timers[i].delay_ms * (1 + missed_ticks);
				m_timers[i].last_fire_ms = now;
			}
			else
			{
				// One-shot timer: mark for removal
				timers_to_remove.push_back(m_timers[i].id);
			}
		}
	}

	// Remove one-shot timers that have fired
	for (int i = 0; i < (int)timers_to_remove.size(); i++)
	{
		cancel(timers_to_remove[i]);
	}
}
