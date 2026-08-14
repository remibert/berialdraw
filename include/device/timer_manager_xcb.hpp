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
	};
};
