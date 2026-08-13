#pragma once
namespace berialdraw
{
	/** Timer event - fired when a timer expires */
	class TimerEvent : public Event
	{
	public:
		/** Constructor
		@param timer_id Unique identifier of the timer
		@param elapsed_ticks Number of timer periods that have elapsed (1+ indicates missed ticks)
		@param widget Timer widget that owns this timer (optional) */
		TimerEvent(uint32_t timer_id, uint32_t elapsed_ticks = 1, Widget* widget = nullptr);

		/** Destructor */
		virtual ~TimerEvent();

		/** Convert to string representation */
		virtual void to_string(String & str) const;

		/** Return the widget (the Timer widget that triggered this event) */
		virtual Widget * widget() const { return m_widget; }

		/** Get the timer identifier */
		uint32_t timer_id() const;

		/** Get the number of elapsed timer periods (1 = normal tick, 2+ = missed ticks caught up) */
		uint32_t elapsed_ticks() const;

		/** Get the event type identifier */
		static uint32_t type_id();

	private:
/// @cond DOXYGEN_IGNORE
		uint32_t m_timer_id;
		uint32_t m_elapsed_ticks;
		Widget* m_widget;
/// @endcond
	};
};
