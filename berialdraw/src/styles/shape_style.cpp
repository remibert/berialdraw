#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ShapeStyle::ShapeStyle()
{
	m_radius = 0;
	m_thickness = 0;
}

/** Serialize the content of widget into json */
void ShapeStyle::serialize(JsonIterator & it)
{
	it["radius_"]      = m_radius;
	it["thickness_"]   = m_thickness;
}

/** Unserialize the content of widget from json */
void ShapeStyle::unserialize(JsonIterator & it)
{
	//m_radius      = (int)(it["radius_"]      | (int)m_radius);
	//m_thickness   = (int)(it["thickness_"]   | (int)m_thickness);
	berialdraw::unserialize("radius_"   , it, m_radius);
	berialdraw::unserialize("thickness_", it, m_thickness);
}

/** Set properties with another */
void ShapeStyle::set(const ShapeStyle & other)
{
	m_radius      = other.m_radius;
	m_thickness   = other.m_thickness;
	UIManager::invalidator()->dirty(this);
}

Style * ShapeStyle::create()
{
	return new ShapeStyle;
}

/** Indicates if the window must be refreshed */
bool ShapeStyle::is_dirty()
{
	return UIManager::invalidator()->is_dirty(this);
}

/** Get the thickness */
Dim ShapeStyle::thickness() const
{
	return m_thickness >> 6;
}

/** Get the thickness with a precision of 64th of a pixel */
Dim ShapeStyle::thickness_() const
{
	return m_thickness;
}

/** Set the thickness in pixels */
void ShapeStyle::thickness(Dim v)
{
	UIManager::invalidator()->dirty(this);
	m_thickness = v << 6;
}

/** Set the thickness with a precision of 64th of a pixel */
void ShapeStyle::thickness_(Dim v)
{
	UIManager::invalidator()->dirty(this);
	m_thickness = v;
}



/** Get the point1 */
const Point & ShapeStyle::point1() const
{
	return m_point1;
}

/** Set the point1 */
void ShapeStyle::point1(const Point & point1_)
{
	UIManager::invalidator()->dirty(this);
	m_point1 = point1_;
}

/** Set the point1 with x and y in pixels */
void ShapeStyle::point1(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	m_point1.set(x,y);
}

/** Set the point1 with a precision of 64th of a pixel */
void ShapeStyle::point1_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	m_point1.set_(x,y);
}



/** Get the point2 */
const Point & ShapeStyle::point2() const
{
	return m_point2;
}

/** Set the point2 */
void ShapeStyle::point2(const Point & point2_)
{
	UIManager::invalidator()->dirty(this);
	m_point2 = point2_;
}

/** Set the point2 with x and y in pixels */
void ShapeStyle::point2(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	m_point2.set(x,y);
}

/** Set the point2 with a precision of 64th of a pixel */
void ShapeStyle::point2_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this);
	m_point2.set_(x,y);
}


/** Get the radius */
Dim ShapeStyle::radius() const
{
	return m_radius >> 6;
}

/** Get the radius with a precision of 64th of a pixel */
Dim ShapeStyle::radius_() const
{
	return m_radius;
}

/** Set the radius in pixels */
void ShapeStyle::radius(Dim v)
{
	UIManager::invalidator()->dirty(this);
	m_radius =  v << 6;
}

/** Set the radius with a precision of 64th of a pixel */
void ShapeStyle::radius_(Dim v)
{
	UIManager::invalidator()->dirty(this);
	m_radius = v;
}