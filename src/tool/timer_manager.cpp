#include "berialdraw_imp.hpp"
#include "device/device_win32.hpp"
#include "device/timer_manager_win32.hpp"

#if defined(USE_SDL)
#include "device/device_sdl.hpp"
#include "device/timer_manager_sdl.hpp"
#endif

#if defined(LINUX) && !defined(USE_SDL)
#include "device/device_xcb.hpp"
#include "device/timer_manager_xcb.hpp"
#endif

#if defined(__APPLE__)
#include "device/device_cocoa.hpp"
#include "device/timer_manager_cocoa.hpp"
#endif

#if defined(LINUX) || defined(__unix__)
#include "device/device_wayland.hpp"
#include "device/timer_manager_wayland.hpp"
#endif

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

#ifdef USE_SDL
		DeviceSdl * sdl_device = dynamic_cast<DeviceSdl *>(device);
		if (sdl_device)
		{
			result = new TimerManagerSdl();
			return result;
		}
#endif

#ifdef __APPLE__
		DeviceCocoa * cocoa_device = dynamic_cast<DeviceCocoa *>(device);
		if (cocoa_device)
		{
			result = new TimerManagerCocoa();
			return result;
		}
#endif

#if defined(LINUX) && !defined(USE_SDL)
		DeviceXcb * xcb_device = dynamic_cast<DeviceXcb *>(device);
		if (xcb_device)
		{
			result = new TimerManagerXcb();
			return result;
		}
#endif

#if defined(LINUX) || defined(__unix__)
		DeviceWayland * wayland_device = dynamic_cast<DeviceWayland *>(device);
		if (wayland_device)
		{
			result = new TimerManagerWayland();
			return result;
		}
#endif

		// Fallback: if no platform-specific device found, try polling
#if defined(LINUX) && !defined(USE_SDL)
		result = new TimerManagerXcb();
#elif defined(LINUX) || defined(__unix__)
		result = new TimerManagerWayland();
#endif
	}

	return result;
}
