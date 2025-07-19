#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
RoundStyle::RoundStyle()
{
	m_radius = 0;
	m_thickness = 0;
}

/** Serialize the content of widget into json */
void RoundStyle::serialize(JsonIterator & it)
{
	it["radius_"]      = m_radius;
	it["thickness_"]   = m_thickness;
}

/** Unserialize the content of widget from json */
void RoundStyle::unserialize(JsonIterator & it)
{
	berialdraw::unserialize("radius_"   , it, m_radius);
	berialdraw::unserialize("thickness_", it, m_thickness);
}

/** Set properties with another */
void RoundStyle::set(const RoundStyle & other)
{
	if (this != &other)
	{
		m_radius      = other.m_radius;
		m_thickness   = other.m_thickness;
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	}
}

/** Copy operator */
RoundStyle& RoundStyle::operator=(const RoundStyle& other)
{
	set(other);
	return *this;
}

Style * RoundStyle::create()
{
	return new RoundStyle;
}

/** Get the thickness */
Dim RoundStyle::thickness() const
{
	return m_thickness >> 6;
}

/** Get the thickness with a precision of 64th of a pixel */
Dim RoundStyle::thickness_() const
{
	return m_thickness;
}

/** Set the thickness in pixels */
void RoundStyle::thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_thickness = v << 6;
}

/** Set the thickness with a precision of 64th of a pixel */
void RoundStyle::thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_thickness = v;
}




/** Get the radius */
Dim RoundStyle::radius() const
{
	return m_radius >> 6;
}

/** Get the radius with a precision of 64th of a pixel */
Dim RoundStyle::radius_() const
{
	return m_radius;
}

/** Set the radius in pixels */
void RoundStyle::radius(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_radius =  v << 6;
}

/** Set the radius with a precision of 64th of a pixel */
void RoundStyle::radius_(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_radius = v;
}