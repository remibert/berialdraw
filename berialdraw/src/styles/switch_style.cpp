#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
SwitchStyle::SwitchStyle()
{
}

/** Serialize the content of widget into json */
void SwitchStyle::serialize(JsonIterator & it)
{
	it["thumb-padding_"]      = m_thumb_padding;
	it["on-track-color"]     = m_on_track_color;
	it["off-track-color"]    = m_off_track_color;
	it["thumb-color"]        = m_thumb_color;
	m_switch_size.serialize ("switch-size",it);
}

/** Unserialize the content of widget from json */
void SwitchStyle::unserialize(JsonIterator & it)
{
	m_switch_size.unserialize("switch-size",it);
	m_on_track_color       = (int)(it["on-track-color"]        | (int)m_on_track_color);
	m_off_track_color      = (int)(it["off-track-color"]       | (int)m_off_track_color);
	m_thumb_color          = (int)(it["thumb-color"]           | (int)m_thumb_color);
	berialdraw::unserialize("thumb-padding_"   , it, m_thumb_padding);
}

/** Copy operator */
SwitchStyle& SwitchStyle::operator=(const SwitchStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void SwitchStyle::set(const SwitchStyle & other)
{
	if (this != &other)
	{
		m_thumb_color        = other.m_thumb_color;
		m_thumb_padding      = other.m_thumb_padding;
		m_on_track_color     = other.m_on_track_color;
		m_off_track_color    = other.m_off_track_color;
		m_switch_size        = other.m_switch_size;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * SwitchStyle::create()
{
	return new SwitchStyle;
}

/** Set the on track color */
void SwitchStyle::on_track_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_on_track_color = col;
}

/** Set the on track color with alpha */
void SwitchStyle::on_track_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_on_track_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the off track color */
void SwitchStyle::off_track_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_off_track_color = col;
}

/** Set the off track color with alpha */
void SwitchStyle::off_track_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_off_track_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the switch color */
void SwitchStyle::thumb_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_thumb_color = col;
}

/** Set the switch color with alpha */
void SwitchStyle::thumb_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_thumb_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the switch size */
void SwitchStyle::switch_size(const Size & size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_switch_size = size_;
}

/** Set the switch size with width and height in pixels */
void SwitchStyle::switch_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_switch_size.set(w,h);
}

/** Set the switch size with a precision of 64th of a pixel */
void SwitchStyle::switch_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_switch_size.set_(w,h);
}

/** Set the thumb padding in pixels */
void SwitchStyle::thumb_padding(Dim pad)
{
	m_thumb_padding = pad<<6;
}
