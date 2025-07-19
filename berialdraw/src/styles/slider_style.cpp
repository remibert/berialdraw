#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
SliderStyle::SliderStyle()
{
}

/** Serialize the content of widget into json */
void SliderStyle::serialize(JsonIterator & it)
{
	it["track-color"]  = m_track_color;
	it["handle-color"] = m_handle_color;

	//it["track-size_"]  = (int)m_track_size;
	//it["handle-size_"] = (int)m_handle_size;

	berialdraw::unserialize("track-size_"   , it, m_track_size);
	berialdraw::unserialize("handle-size_"   , it, m_handle_size);


	it["value"]        = (int)m_value;
	it["min-value"]    = (int)m_min_value;
	it["max-value"]    = (int)m_max_value;
	it["step-value"]   = (int)m_step_value;
}

/** Unserialize the content of widget from json */
void SliderStyle::unserialize(JsonIterator & it)
{
	m_track_color   = (int)(it["track-color"]  | (int)m_track_color);
	m_handle_color  = (int)(it["handle-color"] | (int)m_handle_color);
	m_track_size    = (int)(it["track-size_"]  | (int)m_track_size);
	m_handle_size   = (int)(it["handle-size_"] | (int)m_handle_size);
	m_value         = (int)(it["value"]        | (int)m_value);
	m_min_value     = (int)(it["min-value"]    | (int)m_min_value);
	m_max_value     = (int)(it["max-value"]    | (int)m_max_value);
	m_step_value    = (int)(it["step-value"]   | (int)m_step_value);
	check_slider();
}

/** Copy operator */
SliderStyle& SliderStyle::operator=(const SliderStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void SliderStyle::set(const SliderStyle & other)
{
	if (this != &other)
	{
		m_track_color   = other.m_track_color;
		m_handle_color  = other.m_handle_color;
		m_track_size    = other.m_track_size;
		m_handle_size   = other.m_handle_size;
		m_value         = other.m_value;
		m_min_value     = other.m_min_value;
		m_max_value     = other.m_max_value;
		m_step_value    = other.m_step_value;
		check_slider();
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	}
}

Style * SliderStyle::create()
{
	return new SliderStyle;
}


/** Get the track color */
uint32_t SliderStyle::track_color() const
{
	return m_track_color;
}

/** Set the track color */
void SliderStyle::track_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_track_color = col;
}

/** Set the track color with alpha */
void SliderStyle::track_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_track_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Get the track size */
Dim SliderStyle::track_size() const
{
	return m_track_size>>6;
}

/** Set the track size in pixels */
void SliderStyle::track_size(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_track_size = size_<<6;
}

/** Set the track size with a precision of 64th of a pixel */
void SliderStyle::track_size_(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_track_size = size_;
}

/** Get the handle color */
uint32_t SliderStyle::handle_color() const
{
	return m_handle_color;
}

/** Set the slider color */
void SliderStyle::handle_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_handle_color = col;
}

/** Set the slider color with alpha */
void SliderStyle::handle_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_handle_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Get the handle size */
Dim SliderStyle::handle_size() const
{
	return m_handle_size>>6;
}

/** Set the handle size in pixels */
void SliderStyle::handle_size(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_handle_size = size_<<6;
}

/** Set the handle size with a precision of 64th of a pixel */
void SliderStyle::handle_size_(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_handle_size = size_;
}

/** Get the value of slider */
int32_t SliderStyle::value() const
{
	return m_value;
}

/** Set the value of slider */
void SliderStyle::value(int32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_value = val;
}

/** Get the min value of slider */
int32_t SliderStyle::min_value() const
{
	return m_min_value;
}

/** Set the min value of slider */
void SliderStyle::min_value(int32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_min_value = val;
}

/** Get the max value of slider */
int32_t SliderStyle::max_value() const
{
	return m_max_value;
}

/** Set the max value of slider */
void SliderStyle::max_value(int32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_max_value = val;
}

/** Get the step value of slider */
uint32_t SliderStyle::step_value() const
{
	return m_step_value;
}

/** Set the step value of slider */
void SliderStyle::step_value(uint32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_step_value = val;
}


void SliderStyle::check_slider()
{
	if (m_min_value > m_max_value)
	{
		int32_t tmp = m_max_value;
		m_max_value = m_min_value;
		m_min_value = tmp;
	}

	if (m_value < m_min_value)
	{
		m_value = m_min_value;
	}

	if (m_value > m_max_value)
	{
		m_value = m_max_value;
	}
}


