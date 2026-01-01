#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ScrollViewStyle::ScrollViewStyle()
{
	m_scroll_size.clean();
	m_scroll_position.set(0, 0);
	m_viewport_size.clean();
}

/** Serialize the content of widget into json */
void ScrollViewStyle::serialize(JsonIterator & it)
{
	m_scroll_size.serialize("scroll-size", it);
	m_scroll_position.serialize("scroll-position", it);
	it["scroll-direction"] = (int)m_scroll_direction;
	m_viewport_size.serialize("viewport-size", it);
}

/** Unserialize the content of widget from json */
void ScrollViewStyle::unserialize(JsonIterator & it)
{
	m_scroll_size.unserialize("scroll-size", it);
	m_scroll_position.unserialize("scroll-position", it);
	m_scroll_direction = (ScrollDirection)(int)(it["scroll-direction"] | (int)m_scroll_direction);
	m_viewport_size.unserialize("viewport-size", it);
}

/** Copy operator */
ScrollViewStyle& ScrollViewStyle::operator=(const ScrollViewStyle& other)
{
	if (this != &other)
	{
		set(other);
	}
	return *this;
}

/** Set properties with another */
void ScrollViewStyle::set(const ScrollViewStyle & other)
{
	m_scroll_size     = other.m_scroll_size;
	m_scroll_position = other.m_scroll_position;
	m_scroll_direction = other.m_scroll_direction;
	m_viewport_size  = other.m_viewport_size;
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

Style * ScrollViewStyle::create()
{
	return new ScrollViewStyle;
}

/** Get the scroll direction */
ScrollDirection ScrollViewStyle::scroll_direction() const
{
	return m_scroll_direction;
}

/** Set the scroll direction */
void ScrollViewStyle::scroll_direction(ScrollDirection dir)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_direction = dir;
}
