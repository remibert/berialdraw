#pragma once
namespace berialdraw
{
	class Widget;
	/** Slide event. */
	class SlideEvent : public Event
	{
	public:
		/** Constructor. 
		@param value value of slider 
		@param clicked widget which is clicked */
		SlideEvent(int32_t value, Widget * clicked=0);

		/** Destructor. */
		virtual ~SlideEvent();

		/**  To string.
		 * @param str String to append to. */
		virtual void to_string(String & str) const;

		/** Get type. */
		static uint32_t type_id();

		/** Return the value */
		int32_t value() const;

		/** Return the value widget */
		Widget * widget() const;

	private:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		int32_t m_value = 0;
		Widget * m_widget = 0;
/// @endcond
#endif
	};
};
