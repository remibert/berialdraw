#pragma once
namespace berialdraw
{
	class Widget;
	/** Focused or unfocused event. */
	class FocusEvent : public Event
	{
	public:
		/** Constructor. 
		@param focused focused state
		@param widget widget which is focused */
		FocusEvent(bool focused, Widget * widget=0);

		/** Destructor. */
		virtual ~FocusEvent();

		/**  To string.
		 * @param str String to append to. */
		virtual void to_string(String & str) const;

		/** Get type. */
		static uint32_t type_id();

		/** Return the focused state */
		bool focused() const;

		/** Return the widget */
		Widget * widget() const;

	private:
/// @cond DOXYGEN_IGNORE
		bool m_focused = false;
		Widget * m_widget = 0;
/// @endcond 
	};
};
