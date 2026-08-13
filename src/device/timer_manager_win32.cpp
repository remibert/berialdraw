#include "berialdraw_imp.hpp"
#include "device/timer_manager_win32.hpp"
#include "device/system_time.hpp"
#include <windows.h>
using namespace berialdraw;

// Win32 timer manager initialization
TimerManagerWin32::TimerManagerWin32(void * hwnd)
	: m_hwnd(hwnd)
{
}

// Win32 timer manager cleanup
TimerManagerWin32::~TimerManagerWin32()
{
	// Call KillTimer for all registered timers
	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		KillTimer((HWND)m_hwnd, (UINT_PTR)m_timers[i].id);
	}
	m_timers.clear();
}

// Schedule a new timer
uint32_t TimerManagerWin32::schedule(uint32_t delay_ms, bool recurring, Widget* widget)
{
	if (delay_ms == 0 || !m_hwnd)
	{
		return 0;  // Invalid interval or no window
	}

	uint32_t timer_id = get_next_timer_id();

	// Call SetTimer - Windows will send WM_TIMER messages to window procedure
	UINT_PTR result = SetTimer((HWND)m_hwnd, (UINT_PTR)timer_id, delay_ms, NULL);
	if (result == 0)
	{
		return 0;  // SetTimer failed
	}

	TimerRecord record;
	record.id = timer_id;
	record.delay_ms = delay_ms;
	record.recurring = recurring;
	record.last_fire_ms = SystemTime::get_time_ms();
	record.widget = widget;

	m_timers.push_back(record);
	return timer_id;
}

// Cancel a timer
bool TimerManagerWin32::cancel(uint32_t timer_id)
{
	int index = find_timer_index(timer_id);
	if (index >= 0)
	{
		// Call KillTimer to stop the Windows timer
		KillTimer((HWND)m_hwnd, (UINT_PTR)timer_id);
		m_timers.remove(index);
		return true;
	}
	return false;
}

// Stop all timers
uint32_t TimerManagerWin32::stop_all()
{
	uint32_t count = (uint32_t)m_timers.size();

	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		KillTimer((HWND)m_hwnd, (UINT_PTR)m_timers[i].id);
	}

	return TimerManagerBase::stop_all();
}

// Check for expired timers (no-op for Win32 - WM_TIMER messages handle it)
void TimerManagerWin32::check_timers()
{
	// Win32 delivers WM_TIMER messages through normal event loop
	// on_timer_message() will be called from window procedure
}

// Handle WM_TIMER message from window procedure
bool TimerManagerWin32::on_timer_message(uint32_t timer_id)
{
	bool found = false;

	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		if (m_timers[i].id == timer_id)
		{
			uint64_t now = SystemTime::get_time_ms();
			uint32_t elapsed_ticks = 1;

			// Calculate missed ticks
			if (m_timers[i].last_fire_ms > 0)
			{
				uint64_t missed_ms = now - (m_timers[i].last_fire_ms + m_timers[i].delay_ms);
				if (missed_ms > 0)
				{
					elapsed_ticks = 1 + (uint32_t)(missed_ms / m_timers[i].delay_ms);
				}
			}

			// Push timer event
			push_timer_event(m_timers[i].id, elapsed_ticks, m_timers[i].widget);
			m_timers[i].last_fire_ms = now;

			if (!m_timers[i].recurring)
			{
				// One-shot: cancel after firing
				cancel(m_timers[i].id);
			}

			found = true;
			break;
		}
	}

	return found;
}
