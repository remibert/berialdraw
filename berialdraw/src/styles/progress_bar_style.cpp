#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ProgressBarStyle::ProgressBarStyle()
{
}

/** Serialize the content of widget into json */
void ProgressBarStyle::serialize(JsonIterator & it)
{
	it["track-color"]  = m_track_color;
	it["fill-color"]   = m_fill_color;

	berialdraw::unserialize("track-size_", it, m_track_size);
	berialdraw::unserialize("fill-size_" , it, m_fill_size);

	it["value"]        = (int)m_value;
	it["min-value"]    = (int)m_min_value;
	it["max-value"]    = (int)m_max_value;
	it["step-value"]   = (int)m_step_value;
}

/** Unserialize the content of widget from json */
void ProgressBarStyle::unserialize(JsonIterator & it)
{
	m_track_color   = (int)(it["track-color"]  | (int)m_track_color);
	m_fill_color    = (int)(it["fill-color"] | (int)m_fill_color);
	m_track_size    = (int)(it["track-size_"]  | (int)m_track_size);
	m_fill_size     = (int)(it["fill-size_"] | (int)m_fill_size);
	m_value         = (int)(it["value"]        | (int)m_value);
	m_min_value     = (int)(it["min-value"]    | (int)m_min_value);
	m_max_value     = (int)(it["max-value"]    | (int)m_max_value);
	m_step_value    = (int)(it["step-value"]   | (int)m_step_value);
	check_progress_bar();
}

/** Copy operator */
ProgressBarStyle& ProgressBarStyle::operator=(const ProgressBarStyle& other)
{
	if (this != &other)
	{
		set(other);
	}
	return *this;
}

/** Set properties with another */
void ProgressBarStyle::set(const ProgressBarStyle & other)
{
	m_track_color   = other.m_track_color;
	m_fill_color    = other.m_fill_color;
	m_track_size    = other.m_track_size;
	m_fill_size     = other.m_fill_size;
	m_value         = other.m_value;
	m_min_value     = other.m_min_value;
	m_max_value     = other.m_max_value;
	m_step_value    = other.m_step_value;
	check_progress_bar();
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

Style * ProgressBarStyle::create()
{
	return new ProgressBarStyle;
}

/** Set the track color */
void ProgressBarStyle::track_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_track_color = col;
}

/** Set the track color with alpha */
void ProgressBarStyle::track_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_track_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the track size in pixels */
void ProgressBarStyle::track_size(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_track_size = size_<<6;
}

/** Set the track size with a precision of 64th of a pixel */
void ProgressBarStyle::track_size_(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_track_size = size_;
}

/** Set the progress_bar color */
void ProgressBarStyle::fill_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_fill_color = col;
}

/** Set the progress_bar color with alpha */
void ProgressBarStyle::fill_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_fill_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the fill size in pixels */
void ProgressBarStyle::fill_size(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_fill_size = size_<<6;
}

/** Set the fill size with a precision of 64th of a pixel */
void ProgressBarStyle::fill_size_(Dim size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_fill_size = size_;
}

/** Set the value of progress_bar */
void ProgressBarStyle::value(int32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_value = val;
}

/** Set the min value of progress_bar */
void ProgressBarStyle::min_value(int32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_min_value = val;
}

/** Set the max value of progress_bar */
void ProgressBarStyle::max_value(int32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_max_value = val;
}

/** Set the step value of progress_bar */
void ProgressBarStyle::step_value(uint32_t val)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_step_value = val;
}

void ProgressBarStyle::check_progress_bar()
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
