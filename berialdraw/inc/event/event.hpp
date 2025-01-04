#ifndef Event_hpp_INCLUDED
#define Event_hpp_INCLUDED

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

	private:
/// @cond DOXYGEN_IGNORE
		uint32_t m_type;
/// @endcond
	};
};
#endif
