#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Create timer widget
Timer::Timer(Widget * parent)
	: Widget("timer", parent, sizeof(Timer))
	, m_timer_id(0)
{
	// Timer widgets are invisible by default
	hidden(true);
}

// Destroy timer widget
Timer::~Timer()
{
	// Stop timer if running
	if (m_timer_id != 0)
	{
		stop();
	}
}

// Get style cascade mode for timer
StyleCascadeMode Timer::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

// Copy properties from another timer
void Timer::copy(const Timer & timer)
{
	TimerStyle::set(timer);
}

// Copy properties from another timer pointer
void Timer::copy(const Timer * timer)
{
	if (timer)
	{
		copy(*timer);
	}
}

// Start the timer
void Timer::start()
{
	if (UIManager::is_initialized() && UIManager::timer_manager())
	{
		// Stop previous timer if running
		if (m_timer_id != 0)
		{
			UIManager::timer_manager()->cancel(m_timer_id);
		}

		// Schedule new timer with this widget
		m_timer_id = UIManager::timer_manager()->schedule(m_interval, m_recurring, this);
		m_active = (m_timer_id != 0);
	}
}

// Stop the timer
void Timer::stop()
{
	// Don't try to cancel timer if UIManager is shutting down (timer_manager will be destroyed anyway)
	if (m_timer_id != 0 && UIManager::is_initialized() && !UIManager::is_shutting_down() && UIManager::timer_manager())
	{
		UIManager::timer_manager()->cancel(m_timer_id);
		m_timer_id = 0;
		m_active = false;
	}
}

// Check if timer is currently running
bool Timer::is_running() const
{
	return m_timer_id != 0 && m_active;
}

// Serialize timer to json
void Timer::serialize(JsonIterator & it)
{
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	PaddingStyle::serialize(it);
	TimerStyle::serialize(it);
}

// Unserialize timer from json
void Timer::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	TimerStyle::unserialize(it);
}

// Timer widgets are invisible - no rendering
void Timer::paint(const Region & parent_region)
{
	// Timer widgets do not render anything
}

// Timer widgets have no physical area
void Timer::place(const Area & area, bool in_layout)
{
	// Timer widgets are not placed visually
}

// Timer widgets have no content size
Size Timer::content_size()
{
	return Size(0, 0);
}

// Timer widgets are never hovered
Widget * Timer::hovered(const Region & parent_region, const Point & position)
{
	return 0;
}
