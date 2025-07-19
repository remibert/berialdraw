#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
PieStyle::PieStyle()
{
	m_start_angle = 0;
	m_end_angle = 0;
	m_rope = 0;
}

/** Serialize the content of widget into json */
void PieStyle::serialize(JsonIterator & it)
{
	it["sweep-angle_"] = m_end_angle;
	it["start-angle_"] = m_start_angle;
	it["rope"]        = m_rope      ;
}

/** Unserialize the content of widget from json */
void PieStyle::unserialize(JsonIterator & it)
{
	m_end_angle = (int)(it["sweep-angle_"] | (int)(m_end_angle));
	m_start_angle = (int)(it["start-angle_"] | (int)(m_start_angle));
	m_rope       = (int)(it["rope"]        | (int)(m_rope      ));
}

/** Copy operator */
PieStyle& PieStyle::operator=(const PieStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void PieStyle::set(const PieStyle & other)
{
	if (this != &other)
	{
		m_start_angle = other.m_start_angle;
		m_end_angle = other.m_end_angle;
		m_rope       = other.m_rope;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * PieStyle::create()
{
	return new PieStyle;
}



/** Get the start angle */
Coord PieStyle::start_angle() const
{
	return m_start_angle >> 6;
}

/** Get the start angle with a precision of 64th of a pixel */
Coord PieStyle::start_angle_() const
{
	return m_start_angle;
}

/** Set the start angle in pixels */
void PieStyle::start_angle(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_start_angle = v << 6;
}

/** Set the start angle with a precision of 64th of a pixel */
void PieStyle::start_angle_(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_start_angle = v;
}

	/** Get the end angle */
Coord PieStyle::end_angle() const
{
	return m_end_angle >> 6;
}

/** Get the end angle with a precision of 64th of a pixel */
Coord PieStyle::end_angle_() const
{
	return m_end_angle;
}

/** Set the end angle in pixels */
void PieStyle::end_angle(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_end_angle = v << 6;
}

/** Set the end angle with a precision of 64th of a pixel */
void PieStyle::end_angle_(Coord v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_end_angle = v;
}


	/** Get the rope */
bool PieStyle::rope() const
{
	return m_rope;
}

/** Set the rope */
void PieStyle::rope(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_rope = v;
}