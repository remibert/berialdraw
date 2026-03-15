#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
LineStyle::LineStyle()
{
}

/** Serialize the content of widget into json */
void LineStyle::serialize(JsonIterator & it)
{
	m_point1.serialize(StyleNames::LINE_POINT1,it);
	m_point2.serialize(StyleNames::LINE_POINT2,it);

}

/** Unserialize the content of widget from json */
void LineStyle::unserialize(JsonIterator & it)
{
	m_point1.unserialize(StyleNames::LINE_POINT1,it);
	m_point2.unserialize(StyleNames::LINE_POINT2,it);
}

/** Apply selective style properties from StyleItem (only modifies defined properties) */
void LineStyle::apply_style(StyleItem* item)
{
	if (!item) return;
	
	JsonIterator it = item->properties();
	this->unserialize(it);
}

/** Copy operator */
LineStyle& LineStyle::operator=(const LineStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void LineStyle::set(const LineStyle & other)
{
	if (this != &other)
	{
		m_point1     = other.m_point1;
		m_point2     = other.m_point2;
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}

Style * LineStyle::create()
{
	return new LineStyle;
}

/** Set the point1 */
void LineStyle::point1(const Point & point1_)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_point1 = point1_;
}

/** Set the point1 with x and y in pixels */
void LineStyle::point1(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_point1.set(x,y);
}

/** Set the point1 with a precision of 64th of a pixel */
void LineStyle::point1_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_point1.set_(x,y);
}

/** Set the point2 */
void LineStyle::point2(const Point & point2_)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_point2 = point2_;
}

/** Set the point2 with x and y in pixels */
void LineStyle::point2(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_point2.set(x,y);
}

/** Set the point2 with a precision of 64th of a pixel */
void LineStyle::point2_(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_point2.set_(x,y);
}
