#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ScrollViewStyle::ScrollViewStyle()
{
	m_scroll_size.clean();
	m_scroll_position.set(0, 0);
}

/** Serialize the content of widget into json */
void ScrollViewStyle::serialize(JsonIterator & it)
{
	m_scroll_size.serialize(StyleNames::SCROLLVIEW_SIZE, it);
	m_scroll_position.serialize(StyleNames::SCROLLVIEW_POSITION, it);
	berialdraw::serialize(StyleNames::SCROLLVIEW_DIRECTION, it, m_scroll_direction);
	m_padding.serialize(StyleNames::TEXT_PADDING, it);
}

/** Unserialize the content of widget from json */
void ScrollViewStyle::unserialize(JsonIterator & it)
{
	m_scroll_size.unserialize(StyleNames::SCROLLVIEW_SIZE, it);
	m_scroll_position.unserialize(StyleNames::SCROLLVIEW_POSITION, it);
	berialdraw::unserialize(StyleNames::SCROLLVIEW_DIRECTION, it, m_scroll_direction);
	m_padding.unserialize(StyleNames::TEXT_PADDING, it);
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

/** Copy properties from another without invalidation */
void ScrollViewStyle::copy_from(const ScrollViewStyle & other)
{
	m_scroll_size      = other.m_scroll_size;
	m_scroll_position  = other.m_scroll_position;
	m_scroll_direction = other.m_scroll_direction;
	m_padding          = other.m_padding;
}

/** Set properties with another */
void ScrollViewStyle::set(const ScrollViewStyle & other)
{
	copy_from(other);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

Style * ScrollViewStyle::create()
{
	return new ScrollViewStyle;
}

/** Set the scroll direction */
void ScrollViewStyle::scroll_direction(ScrollDirection dir)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_direction = dir;
}

/** Set the padding */
void ScrollViewStyle::padding(const Margin& m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding = m;
}

/** Set the padding in pixels */
void ScrollViewStyle::padding(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(top, left, bottom, right);
}

/** Set the padding with a precision of 64th of a pixel */
void ScrollViewStyle::padding_(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set_(top, left, bottom, right);
}

/** Set the padding */
void ScrollViewStyle::padding(Dim horizontal, Dim vertical)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(vertical, horizontal, vertical, horizontal);
}

/** Set the padding */
void ScrollViewStyle::padding(Dim value)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(value, value, value, value);
}
