#pragma once
namespace berialdraw
{
	/** The TimerStyle class defines timer-specific properties */
	class TimerStyle : public Style
	{
	public:
		/** Constructor */
		TimerStyle();

		/** Serialize the content into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		TimerStyle& operator=(const TimerStyle& other);

		/** Set properties with another */
		void set(const TimerStyle & other);

		/** Create new timer style */
		static Style * create();

		/** Get the timer interval in milliseconds */
		uint32_t interval() const { return m_interval; }

		/** Set the timer interval in milliseconds */
		void interval(uint32_t ms);

		/** Get whether the timer is recurring */
		bool recurring() const { return m_recurring; }

		/** Set whether the timer should repeat */
		void recurring(bool value);

		/** Get whether the timer is active */
		bool active() const { return m_active; }

		/** Set whether the timer is active */
		void active(bool value);

		/** Get the property name for this style */
		const char* property_name() const override { return "timer"; }

	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_interval;   ///< Timer interval in milliseconds
		bool     m_recurring;  ///< True if timer repeats, false for one-shot
		bool     m_active;     ///< True if timer is running
/// @endcond
	};
};
