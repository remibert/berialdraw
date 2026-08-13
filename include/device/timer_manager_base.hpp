#pragma once
namespace berialdraw
{
	class Widget;
	class TimerEvent;

	/** Base class for timer manager implementations
	Provides common data structures and methods shared across all platform-specific implementations */
	class TimerManagerBase : public TimerManager
	{
	public:
		/** Destructor */
		virtual ~TimerManagerBase() {}

		/** Cancel a scheduled timer
		@param timer_id The identifier returned by schedule()
		@return True if timer was found and cancelled, false otherwise */
		virtual bool cancel(uint32_t timer_id) override;

		/** Stop all active timers
		@return Number of timers cancelled */
		virtual uint32_t stop_all() override;

	protected:
/// @cond DOXYGEN_IGNORE
		/** Timer record structure - common to all implementations */
		struct TimerRecord
		{
			uint32_t id;              ///< Unique timer identifier
			uint32_t delay_ms;        ///< Timer interval in milliseconds
			bool recurring;           ///< True if repeating timer
			uint64_t deadline_ms;     ///< Next deadline in milliseconds (for polling)
			uint64_t last_fire_ms;    ///< Last time timer fired
			Widget* widget;           ///< Timer widget that owns this timer
		};

		/** Timer records list */
		Vector<TimerRecord> m_timers;

		/** Next timer ID counter */
		uint32_t m_next_timer_id = 1;

		/** Get next unique timer ID
		@return New unique timer identifier */
		uint32_t get_next_timer_id();

		/** Find timer record by ID
		@param timer_id The timer identifier
		@return Pointer to timer record, or nullptr if not found */
		TimerRecord* find_timer(uint32_t timer_id);

		/** Find timer index by ID
		@param timer_id The timer identifier
		@return Index in m_timers array, or -1 if not found */
		int find_timer_index(uint32_t timer_id);
/// @endcond
	};
}
