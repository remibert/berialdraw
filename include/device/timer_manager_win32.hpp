#pragma once
#include "timer_manager_base.hpp"
namespace berialdraw
{
	/** Win32 timer manager using native SetTimer and WM_TIMER messages
	Leverages Windows message queue for efficient timer handling */
	class TimerManagerWin32 : public TimerManagerBase
	{
	public:
		/** Constructor
		@param hwnd Window handle for timer registration */
		TimerManagerWin32(void * hwnd);

		/** Destructor */
		virtual ~TimerManagerWin32();

		/** Schedule a timer
		@param delay_ms Timer interval in milliseconds
		@param recurring If true, timer repeats
		@param widget Timer widget that owns this timer
		@return Unique timer identifier */
		virtual uint32_t schedule(uint32_t delay_ms, bool recurring = false, Widget* widget = nullptr) override;



		/** Check for expired timers (no-op for Win32, messages handle this) */
		virtual void check_timers() override;

		/** Cancel a timer
		@param timer_id The identifier to cancel
		@return True if cancelled */
		virtual bool cancel(uint32_t timer_id) override;

		/** Stop all timers
		@return Number of timers cancelled */
		virtual uint32_t stop_all() override;

		/** Handle WM_TIMER message - should be called from window procedure
		@param wparam Timer ID from WM_TIMER
		@return True if timer was found */
		bool on_timer_message(uint32_t timer_id);

	private:
/// @cond DOXYGEN_IGNORE
		void * m_hwnd;                      ///< Window handle
/// @endcond
	};
};
