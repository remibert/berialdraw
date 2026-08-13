#include "berialdraw_imp.hpp"
#include "device/timer_manager_sdl.hpp"
#include "device/system_time.hpp"
#include <SDL3/SDL.h>
using namespace berialdraw;

// SDL timer manager initialization
TimerManagerSdl::TimerManagerSdl()
{
}

// SDL3 timer manager cleanup
TimerManagerSdl::~TimerManagerSdl()
{
	// Remove all SDL timers
	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		if (m_timers[i].sdl_timer_id)
		{
			SDL_RemoveTimer((SDL_TimerID)m_timers[i].sdl_timer_id);
		}
	}
	m_timers.clear();
}

// Static callback for SDL timer - called by SDL event loop
uint32_t TimerManagerSdl::timer_callback(uint32_t interval, void * param)
{
	TimerRecord * record = (TimerRecord*)param;
	if (!record)
	{
		return 0;  // Stop timer
	}

	// Push timer event
	if (UIManager::is_initialized() && UIManager::timer_manager())
	{
		uint64_t now = SystemTime::get_time_ms();

		uint32_t elapsed_ticks = 1;
		if (record->last_fire_ms > 0)
		{
			uint64_t missed_ms = now - (record->last_fire_ms + record->delay_ms);
			if (missed_ms > 0)
			{
				elapsed_ticks = 1 + (uint32_t)(missed_ms / record->delay_ms);
			}
		}

		((TimerManagerSdl*)UIManager::timer_manager())->push_timer_event(record->id, elapsed_ticks, record->widget);
		record->last_fire_ms = now;
	}

	// Return interval to continue timer, or 0 to stop
	return record->recurring ? interval : 0;
}

// Schedule a new timer
uint32_t TimerManagerSdl::schedule(uint32_t delay_ms, bool recurring, Widget* widget)
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
	record.last_fire_ms = 0;
	record.widget = widget;

	// Create SDL timer
	record.sdl_timer_id = SDL_AddTimer(delay_ms, timer_callback, (void*)&record);
	if (!record.sdl_timer_id)
	{
		return 0;  // Timer creation failed
	}

	// Store a copy (SDL callback needs stable address)
	m_timers.push_back(record);

	// Update the stored record's sdl_timer_id (pointer stability)
	m_timers.back().sdl_timer_id = record.sdl_timer_id;

	return timer_id;
}

// Cancel a timer
bool TimerManagerSdl::cancel(uint32_t timer_id)
{
	int index = find_timer_index(timer_id);
	if (index >= 0)
	{
		// Remove SDL timer
		if (m_timers[index].sdl_timer_id)
		{
			SDL_RemoveTimer((SDL_TimerID)m_timers[index].sdl_timer_id);
		}

		m_timers.remove(index);
		return true;
	}
	return false;
}

// Stop all timers
uint32_t TimerManagerSdl::stop_all()
{
	uint32_t count = 0;

	for (int i = 0; i < (int)m_timers.size(); i++)
	{
		if (m_timers[i].sdl_timer_id)
		{
			SDL_RemoveTimer((SDL_TimerID)m_timers[i].sdl_timer_id);
			count++;
		}
	}

	return TimerManagerBase::stop_all();
}

// Check for expired timers (no-op for SDL - callbacks handle it)
void TimerManagerSdl::check_timers()
{
	// SDL handles timers via callbacks in its event loop
	// No polling needed here
}
