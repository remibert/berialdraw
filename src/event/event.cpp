#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Event::Event (uint32_t typ)
{
	m_type = typ;
}

/** Destructor */
Event::~Event()
{
}

/** Return the event type */
uint32_t Event::type() const
{
	return m_type;
}
