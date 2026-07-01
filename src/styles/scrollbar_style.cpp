#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ScrollbarStyle::ScrollbarStyle()
{
}

/** Serialize the content of style into json */
void ScrollbarStyle::serialize(JsonIterator & it)
{
	it[StyleNames::SCROLLBAR_VISIBLE    ] = m_scrollbar_visible;
	it[StyleNames::SCROLLBAR_THUMB_COLOR] = (int)m_scrollbar_thumb_color;
	it[StyleNames::SCROLLBAR_WIDTH      ] = (int)scrollbar_width();
	it[StyleNames::SCROLLBAR_RADIUS     ] = (int)scrollbar_radius();
	it[StyleNames::SCROLLBAR_MARGIN     ]      = (int)scrollbar_margin();
}

/** Unserialize the content of style from json */
void ScrollbarStyle::unserialize(JsonIterator & it)
{
	m_scrollbar_visible     = (bool)(it[StyleNames::SCROLLBAR_VISIBLE] | m_scrollbar_visible);
	m_scrollbar_thumb_color = (int)(it[StyleNames::SCROLLBAR_THUMB_COLOR] | (int)m_scrollbar_thumb_color);
	
	int width = (int)(it[StyleNames::SCROLLBAR_WIDTH] | (int)scrollbar_width());
	m_scrollbar_width = width << 6;
	
	int radius = (int)(it[StyleNames::SCROLLBAR_RADIUS] | (int)scrollbar_radius());
	m_scrollbar_radius = radius << 6;
	
	int margin = (int)(it[StyleNames::SCROLLBAR_MARGIN] | (int)scrollbar_margin());
	m_scrollbar_margin = margin << 6;
}

/** Copy operator */
ScrollbarStyle& ScrollbarStyle::operator=(const ScrollbarStyle& other)
{
	if (this != &other)
	{
		set(other);
	}
	return *this;
}

/** Copy properties from another without invalidation */
void ScrollbarStyle::copy_from(const ScrollbarStyle & other)
{
	m_scrollbar_visible     = other.m_scrollbar_visible;
	m_scrollbar_thumb_color = other.m_scrollbar_thumb_color;
	m_scrollbar_width       = other.m_scrollbar_width;
	m_scrollbar_radius      = other.m_scrollbar_radius;
	m_scrollbar_margin      = other.m_scrollbar_margin;
}

/** Set properties with another */
void ScrollbarStyle::set(const ScrollbarStyle & other)
{
	copy_from(other);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

/** Create new scrollbar style */
Style * ScrollbarStyle::create()
{
	return new ScrollbarStyle;
}

/** Set whether the scrollbar is visible */
void ScrollbarStyle::scrollbar_visible(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_scrollbar_visible = v;
}

/** Get the scrollbar thumb color */
uint32_t ScrollbarStyle::scrollbar_thumb_color() const
{
	return UIManager::colors()->color(m_scrollbar_thumb_color, false);
}

/** Set the scrollbar thumb color */
void ScrollbarStyle::scrollbar_thumb_color(uint32_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_scrollbar_thumb_color = c;
}

/** Set the scrollbar width in pixels */
void ScrollbarStyle::scrollbar_width(Dim w)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scrollbar_width = w << 6;
}

/** Set the scrollbar width in 64ths of pixels */
void ScrollbarStyle::scrollbar_width_q6(Dim w)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scrollbar_width = w;
}

/** Set the scrollbar radius in pixels */
void ScrollbarStyle::scrollbar_radius(Dim r)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_scrollbar_radius = r << 6;
}

/** Set the scrollbar radius in 64ths of pixels */
void ScrollbarStyle::scrollbar_radius_q6(Dim r)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_scrollbar_radius = r;
}

/** Set the scrollbar margin in pixels */
void ScrollbarStyle::scrollbar_margin(Dim m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scrollbar_margin = m << 6;
}

/** Set the scrollbar margin in 64ths of pixels */
void ScrollbarStyle::scrollbar_margin_q6(Dim m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scrollbar_margin = m;
}
