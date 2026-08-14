#include "berialdraw_imp.hpp"

#if defined(WIN32)
	#include "device/device_win32.hpp"
	#include "device/timer_manager_win32.hpp"
#endif

#if defined(LINUX)
	#include "device/device_xcb.hpp"
#endif

#if defined(__APPLE__)
	#include "device/device_cocoa.hpp"
#endif

#include "device/timer_manager_base.hpp"

using namespace berialdraw;

// Push a timer event to the notifier
void TimerManager::push_timer_event(uint32_t timer_id, uint32_t elapsed_ticks, Widget* widget)
{
	if (UIManager::is_initialized() && UIManager::notifier())
	{
		UIManager::notifier()->push_event(new TimerEvent(timer_id, elapsed_ticks, widget));
	}
}

// Factory method to create appropriate timer manager for the device
TimerManager * TimerManager::create(Device * device)
{
	TimerManager * result = 0;

	if (device)
	{
		// Try to detect device type using dynamic_cast
		// Platform-specific implementations are conditionally compiled

#ifdef WIN32
		DeviceWin32 * win32_device = dynamic_cast<DeviceWin32 *>(device);
		if (win32_device)
		{
			result = new TimerManagerWin32(win32_device->hwnd());
			return result;
		}
#endif

#ifdef __APPLE__
		DeviceCocoa * cocoa_device = dynamic_cast<DeviceCocoa *>(device);
		if (cocoa_device)
		{
			result = new TimerManagerBase();
			return result;
		}
#endif

#ifdef LINUX
		DeviceXcb * xcb_device = dynamic_cast<DeviceXcb *>(device);
		if (xcb_device)
		{
			result = new TimerManagerBase();
			return result;
		}
#endif

		// Fallback: if no platform-specific device found, use base implementation
		result = new TimerManagerBase();
	}

	return result;
}
