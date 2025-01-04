#ifndef ScrollEvent_hpp_INCLUDED
#define ScrollEvent_hpp_INCLUDED

namespace berialdraw
{
	class ScrollView;
	/** Scrolling event */
	class ScrollEvent : public Event
	{
	public:
		/** Constructor */
		ScrollEvent(const Point & shift, ScrollView * scroll_view=0);

		/** Destructor */
		virtual ~ScrollEvent();

		/** To string */
		virtual void to_string(String & str) const;

		/** Get the shift of scrolling */
		const Point & shift() const;

		/** Return the scroll view widget */
		ScrollView * scroll_view() const;

		/** Get type */
		static uint32_t type_id();

	private:
/// @cond DOXYGEN_IGNORE
		Point m_shift;
		ScrollView * m_scroll_view = 0;
/// @endcond
	};
};
#endif
