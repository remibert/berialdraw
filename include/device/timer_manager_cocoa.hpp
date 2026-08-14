#pragma once
#include "timer_manager_base.hpp"
namespace berialdraw
{
	/** Cocoa (macOS) timer manager using software polling
	Checks for expired timers during Device::dispatch() */
	class TimerManagerCocoa : public TimerManagerBase
	{
	public:
		/** Constructor */
		TimerManagerCocoa();

		/** Destructor */
		virtual ~TimerManagerCocoa();
	};
};
