#ifndef CheckEvent_hpp_INCLUDED
#define CheckEvent_hpp_INCLUDED

namespace berialdraw
{
	class Widget;
	/** Clicking event. */
	class CheckEvent : public Event
	{
	public:
		/** Constructor. 
		@param checked checked state
		@param clicked widget which is clicked */
		CheckEvent(bool checked, Widget * clicked=0);

		/** Destructor. */
		virtual ~CheckEvent();

		/**  To string.
		 * @param str String to append to. */
		virtual void to_string(String & str) const;

		/** Get type. */
		static uint32_t type_id();

		/** Return the checked state */
		bool checked() const;

		/** Return the checked widget */
		Widget * widget() const;

	private:
/// @cond DOXYGEN_IGNORE
		bool m_checked = false;
		Widget * m_widget = 0;
/// @endcond 
	};
};
#endif
