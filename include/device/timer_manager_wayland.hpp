#pragma once
#include "timer_manager_base.hpp"
namespace berialdraw
{
	/** Wayland timer manager using software polling
	Checks for expired timers during Device::dispatch() */
	class TimerManagerWayland : public TimerManagerBase
	{
	public:
		/** Constructor */
		TimerManagerWayland();

		/** Destructor */
		virtual ~TimerManagerWayland();
	};
};
