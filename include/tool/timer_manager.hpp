#pragma once
namespace berialdraw
{
	class Device;
	class TimerEvent;

	/** Abstract timer manager interface for platform-specific timer implementations.
	Each platform (Win32, SDL, XCB, Cocoa, Wayland) provides its own implementation
	using native timers when available or software polling when necessary. */
	class TimerManager
	{
	public:
		/** Destructor */
		virtual ~TimerManager() {}

		/** Schedule a timer to fire after delay_ms
		@param delay_ms Timer interval in milliseconds
		@param recurring If true, timer repeats indefinitely; if false, fires once
		@param widget Timer widget that owns this timer (optional)
		@return Unique timer identifier (0 indicates failure) */
		virtual uint32_t schedule(uint32_t delay_ms, bool recurring = false, Widget* widget = nullptr) = 0;

		/** Cancel a scheduled timer
		@param timer_id The identifier returned by schedule()
		@return True if timer was found and cancelled, false otherwise */
		virtual bool cancel(uint32_t timer_id) = 0;

		/** Stop all active timers
		@return Number of timers cancelled */
		virtual uint32_t stop_all() = 0;

		/** Check for expired timers and generate TimerEvents
		Called periodically from Device::dispatch() before processing native events.
		Platform-native implementations may do nothing here (events come via native callbacks).
		Polling implementations check current time and push TimerEvents for expired timers. */
		virtual void check_timers() = 0;
	/** Create the appropriate TimerManager implementation for the given device
	@param device The Device instance to detect platform type
	@return A new TimerManager instance (caller must manage lifetime) */
	static TimerManager * create(Device * device);
	protected:
/// @cond DOXYGEN_IGNORE
		/** Push a timer event to the notifier
		@param timer_id The timer that expired
		@param elapsed_ticks Number of timer periods that elapsed (1 = normal, 2+ = missed ticks)
		@param widget Timer widget that owns this timer (optional) */
		void push_timer_event(uint32_t timer_id, uint32_t elapsed_ticks, Widget* widget);
/// @endcond
	};
};
