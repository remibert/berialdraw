#pragma once
#include "timer_manager_base.hpp"
namespace berialdraw
{
	/** XCB (X11) timer manager using software polling
	Checks for expired timers during Device::dispatch() */
	class TimerManagerXcb : public TimerManagerBase
	{
	public:
		/** Constructor */
		TimerManagerXcb();

		/** Destructor */
		virtual ~TimerManagerXcb();

		/** Schedule a timer
		@param delay_ms Timer interval in milliseconds
		@param recurring If true, timer repeats
		@param widget Timer widget that owns this timer
		@return Unique timer identifier */
		virtual uint32_t schedule(uint32_t delay_ms, bool recurring = false, Widget* widget = nullptr) override;

		/** Check for expired timers and generate events */
		virtual void check_timers() override;
	};
};
