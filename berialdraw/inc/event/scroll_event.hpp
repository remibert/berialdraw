#pragma once
namespace berialdraw
{
	class ScrollView;
	/** Scrolling event */
	class ScrollEvent : public Event
	{
	public:
		/** Constructor */
		ScrollEvent(const Point & shift, Widget * scroll_content=0);

		/** Destructor */
		virtual ~ScrollEvent();

		/** To string */
		virtual void to_string(String & str) const;

		/** Return the widget */
		virtual Widget * widget() const;

		/** Get the shift of scrolling */
		const Point & shift() const;

		/** Get type */
		static uint32_t type_id();

	private:
/// @cond DOXYGEN_IGNORE
		Point m_shift;
		Widget * m_scrollable_content = 0;
/// @endcond
	};
};
