#pragma once
namespace berialdraw
{
	class Widget;

	/** Clicking event. */
	class ClickEvent : public Event
	{
	public:
		/** Constructor. 
		@param position click position
		@param clicked widget which is clicked */
		ClickEvent(const Point & position, Widget * clicked=0);

		/** Destructor. */
		virtual ~ClickEvent();

		/**  To string.
		 * @param str String to append to. */
		virtual void to_string(String & str) const;

		/** Return the widget */
		virtual Widget * widget() const;

		/** Get type. */
		static uint32_t type_id();

		/** Get the position of click. */
		const Point & position() const;

	private:
/// @cond DOXYGEN_IGNORE
		Point m_position; ///< Click position.
		Widget * m_widget = 0;
/// @endcond
	};
};
