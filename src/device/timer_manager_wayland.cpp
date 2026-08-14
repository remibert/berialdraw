#include "berialdraw_imp.hpp"
#include "device/timer_manager_wayland.hpp"
using namespace berialdraw;

// Wayland timer manager initialization
TimerManagerWayland::TimerManagerWayland()
{
}

// Wayland timer manager cleanup
TimerManagerWayland::~TimerManagerWayland()
{
	m_timers.clear();
}
