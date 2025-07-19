#pragma once
namespace berialdraw
{
	/** Touch event */
	class TouchEvent : public Event
	{
	public:
		/** Touch state */
		enum TouchState
		{
			TOUCH_UP   = 0,
			TOUCH_DOWN = 1,
			TOUCH_MOVE = 3,
		};

		/** Constructor */
		TouchEvent(const Point & position, enum TouchState state);

		/** Destructor */
		virtual ~TouchEvent();

		/** To string */
		virtual void to_string(String & str) const;

		/** Return the widget */
		virtual Widget * widget() const {return 0;}

		/** Return the event type */
		TouchState state() const;

		/** Get the position of touch */
		const Point & position() const;

		/** Get type */
		static uint32_t type_id();

	private:
/// @cond DOXYGEN_IGNORE
		Point m_position;
		enum TouchState m_state = TOUCH_UP;
/// @endcond
	};
};
