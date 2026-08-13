#include "berialdraw_imp.hpp"
#include "device/timer_manager_wayland.hpp"
#include "device/system_time.hpp"
using namespace berialdraw;

// Wayland timer manager initialization
TimerManagerWayland::TimerManagerWayland()
{
}

// Wayland timer manager cleanup
TimerManagerWayland::~TimerManagerWayland()
{
	m_timers.clear();
}

// Schedule a new timer
uint32_t TimerManagerWayland::schedule(uint32_t delay_ms, bool recurring, Widget* widget)
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
void TimerManagerWayland::check_timers()
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
				m_timers[i].deadline_ms = now + m_timers[i].delay_ms;
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
