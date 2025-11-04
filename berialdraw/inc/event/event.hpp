#pragma once
namespace berialdraw
{
	/** Abstract class for all events */
	class Event
	{
	public:
		/** Constructor */
		Event (uint32_t typ);

		/** Destructor */
		virtual ~Event();

		/** Return the event type */
		virtual uint32_t type() const;

		/** To string */
		virtual void to_string(String & str) const = 0;

		/** Return the widget */
		virtual Widget * widget() const = 0;

	private:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		uint32_t m_type;
/// @endcond
#endif
	};
};
