#include "berialdraw_imp.hpp"
#include "device/timer_manager_cocoa.hpp"
using namespace berialdraw;

// Cocoa timer manager initialization
TimerManagerCocoa::TimerManagerCocoa()
{
}

// Cocoa timer manager cleanup
TimerManagerCocoa::~TimerManagerCocoa()
{
	m_timers.clear();
}
