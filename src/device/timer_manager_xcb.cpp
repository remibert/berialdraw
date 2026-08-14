#include "berialdraw_imp.hpp"
#include "device/timer_manager_xcb.hpp"
using namespace berialdraw;

// XCB timer manager initialization
TimerManagerXcb::TimerManagerXcb()
{
}

// XCB timer manager cleanup
TimerManagerXcb::~TimerManagerXcb()
{
	m_timers.clear();
}
