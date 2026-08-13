#pragma once
#include "timer_manager_base.hpp"
namespace berialdraw
{
	/** SDL3 timer manager using native SDL_AddTimer callbacks
	Leverages SDL's event loop for efficient timer handling */
	class TimerManagerSdl : public TimerManagerBase
	{
	public:
		/** Constructor */
		TimerManagerSdl();

		/** Destructor */
		virtual ~TimerManagerSdl();

		/** Schedule a timer
		@param delay_ms Timer interval in milliseconds
		@param recurring If true, timer repeats
		@param widget Timer widget that owns this timer
		@return Unique timer identifier */
		virtual uint32_t schedule(uint32_t delay_ms, bool recurring = false, Widget* widget = nullptr) override;



		/** Cancel a timer
		@param timer_id The identifier to cancel
		@return True if cancelled */
		virtual bool cancel(uint32_t timer_id) override;

		/** Stop all timers
		@return Number of timers cancelled */
		virtual uint32_t stop_all() override;

		/** Check for expired timers (no-op for SDL, callbacks handle this) */
		virtual void check_timers() override;

	private:
/// @cond DOXYGEN_IGNORE
	/** Static callback for SDL timer events
	@param interval Timer interval in milliseconds
	@param param Pointer to TimerRecord */
	static uint32_t timer_callback(uint32_t interval, void * param);/// @endcond
	};
};