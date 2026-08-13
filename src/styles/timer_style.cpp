#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Timer style initialization
TimerStyle::TimerStyle()
	: m_interval(1000)
	, m_recurring(false)
	, m_active(false)
{
}

// Create a new timer style instance
Style * TimerStyle::create()
{
	return new TimerStyle();
}

// Copy timer style properties from another
TimerStyle& TimerStyle::operator=(const TimerStyle& other)
{
	if (this != &other)
	{
		set(other);
	}
	return *this;
}

// Set all properties from another timer style
void TimerStyle::set(const TimerStyle & other)
{
	m_interval = other.m_interval;
	m_recurring = other.m_recurring;
	m_active = other.m_active;
}

// Set the timer interval in milliseconds
void TimerStyle::interval(uint32_t ms)
{
	m_interval = ms > 0 ? ms : 1;
}

// Set whether the timer should repeat
void TimerStyle::recurring(bool value)
{
	m_recurring = value;
}

// Set whether the timer is active
void TimerStyle::active(bool value)
{
	m_active = value;
}

// Serialize timer style to json
void TimerStyle::serialize(JsonIterator & it)
{
	it["interval"] = (int)m_interval;
	it["recurring"] = m_recurring;
	it["active"] = m_active;
}

// Unserialize timer style from json
void TimerStyle::unserialize(JsonIterator & it)
{
	m_interval = (int)(it["interval"] | (int)m_interval);
	m_recurring = (bool)(it["recurring"] | m_recurring);
	m_active = (bool)(it["active"] | m_active);
}
