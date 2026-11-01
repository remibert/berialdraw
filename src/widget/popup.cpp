#include "berialdraw_imp.hpp"

using namespace berialdraw;

Popup::Popup(Window * parent):
	Pane("popup", parent, sizeof(Popup))
{
	hidden(true);
}

Popup::~Popup()
{
	Window * window = dynamic_cast<Window*>(m_parent);
	if (m_open)
	{
		close();
	}
	else if (window)
	{
		window->deactivate_popup(this, true);
	}
	m_open = false;
	m_anchor = 0;
	m_point_anchor = false;
}

bool Popup::open(Widget * anchor)
{
	bool result = false;
	Window * window = dynamic_cast<Window*>(m_parent);
	if (window && anchor && anchor->root() == window && contains(anchor) == false)
	{
		m_anchor = anchor;
		m_point_anchor = false;
		activate(window);
		result = true;
	}
	return result;
}

// Open the popup at a point in the same coordinate space as widget positions and click events.
bool Popup::open_at(const Point & position)
{
	bool result = false;
	Window * window = dynamic_cast<Window*>(m_parent);
	if (window && position.is_x_defined() && position.is_y_defined())
	{
		m_anchor = 0;
		m_anchor_position = position;
		m_point_anchor = true;
		activate(window);
		result = true;
	}
	return result;
}

// Activate the popup in its parent window.
void Popup::activate(Window * window)
{
	window->activate_popup(this);
	m_open = true;
	hidden(false);
	move_to_end();
	UIManager::invalidator()->dirty(window, Invalidator::GEOMETRY);
}

void Popup::close()
{
	close_internal(true);
}

void Popup::close_internal(bool scroll_to_focus)
{
	if (m_open)
	{
		m_open = false;
		m_anchor = 0;
		m_point_anchor = false;
		hidden(true);
		Window * window = dynamic_cast<Window*>(m_parent);
		if (window)
		{
			window->deactivate_popup(this, scroll_to_focus);
			UIManager::invalidator()->dirty(window, Invalidator::GEOMETRY);
		}
	}
}

bool Popup::is_open() const
{
	return m_open;
}

bool Popup::contains(Widget * widget) const
{
	bool result = false;
	Widget * current = widget;
	while (current && result == false)
	{
		if (current == this)
		{
			result = true;
		}
		else
		{
			current = current->parent();
		}
	}
	return result;
}

// Place the popup within its parent area.
void Popup::place(const Area & area, bool in_layout)
{
	if (m_open)
	{
		if (m_point_anchor)
		{
			place_open_popup(area, in_layout);
		}
		else if (m_anchor && anchor_is_visible())
		{
			place_open_popup(area, in_layout);
		}
		else
		{
			close();
		}
	}
}

// Check whether the anchor is visible within its clipping ancestors.
bool Popup::anchor_is_visible() const
{
	bool result = false;
	bool visible = !m_anchor->hidden();
	int64_t visible_left = m_anchor->foreclip().x_q6();
	int64_t visible_top = m_anchor->foreclip().y_q6();
	int64_t visible_right = visible_left + m_anchor->foreclip().width_q6();
	int64_t visible_bottom = visible_top + m_anchor->foreclip().height_q6();
	Widget * ancestor = m_anchor->parent();
	bool reached_window = false;

	while (ancestor)
	{
		if (ancestor->hidden())
		{
			visible = false;
		}

		const Area & clip = ancestor == m_parent ? ancestor->foreclip() : ancestor->m_contentclip;
		int64_t left = clip.x_q6();
		int64_t top = clip.y_q6();
		int64_t right = left + clip.width_q6();
		int64_t bottom = top + clip.height_q6();
		if (visible_left < left)
		{
			visible_left = left;
		}
		if (visible_top < top)
		{
			visible_top = top;
		}
		if (visible_right > right)
		{
			visible_right = right;
		}
		if (visible_bottom > bottom)
		{
			visible_bottom = bottom;
		}

		if (visible_right <= visible_left || visible_bottom <= visible_top)
		{
			visible = false;
		}

		if (ancestor == m_parent)
		{
			reached_window = true;
			break;
		}
		ancestor = ancestor->parent();
	}

	if (visible && reached_window)
	{
		result = true;
	}
	return result;
}

// Fit the popup size to the available area.
void Popup::fit_size_to_area(const Area & area, Dim & width, Dim & height)
{
	if (m_min_size.width_q6() > area.width_q6())
	{
		m_min_size.width_q6(area.width_q6());
	}
	if (m_min_size.height_q6() > area.height_q6())
	{
		m_min_size.height_q6(area.height_q6());
	}
	if (m_max_size.width_q6() > area.width_q6())
	{
		m_max_size.width_q6(area.width_q6());
	}
	if (m_max_size.height_q6() > area.height_q6())
	{
		m_max_size.height_q6(area.height_q6());
	}

	Size preferred_size = content_size();
	width = adapt_size(preferred_size.width_q6(),
		m_size.is_width_undefined() ? preferred_size.width_q6() : m_size.width_q6(),
		m_min_size.width_q6(), m_max_size.width_q6());
	height = adapt_size(preferred_size.height_q6(),
		m_size.is_height_undefined() ? preferred_size.height_q6() : m_size.height_q6(),
		m_min_size.height_q6(), m_max_size.height_q6());
}

// Position the popup relative to its widget or point anchor.
void Popup::position_popup(const Area & area, Dim width, Dim height)
{
	int64_t anchor_x = m_point_anchor ? m_anchor_position.x_q6() : m_anchor->foreclip().x_q6();
	int64_t anchor_y = m_point_anchor ? m_anchor_position.y_q6() : m_anchor->foreclip().y_q6();
	int64_t local_x = anchor_x - area.x_q6();
	int64_t anchor_top = anchor_y - area.y_q6();
	int64_t anchor_bottom = m_point_anchor ? anchor_top : anchor_top + m_anchor->foreclip().height_q6();
	int64_t max_x = (int64_t)area.width_q6() - width;
	int64_t max_y = (int64_t)area.height_q6() - height;
	if (max_x < 0)
	{
		max_x = 0;
	}
	if (max_y < 0)
	{
		max_y = 0;
	}
	if (local_x < 0)
	{
		local_x = 0;
	}
	if (local_x > max_x)
	{
		local_x = max_x;
	}

	int64_t below = (int64_t)area.height_q6() - anchor_bottom;
	int64_t above = anchor_top;
	int64_t local_y = anchor_bottom;
	if ((int64_t)height > below && above > below)
	{
		local_y = anchor_top - height;
	}
	if (local_y < 0)
	{
		local_y = 0;
	}
	if (local_y > max_y)
	{
		local_y = max_y;
	}

	Coord x = (Coord)local_x;
	Coord y = (Coord)local_y;
	if (m_position.x_q6() != x || m_position.y_q6() != y)
	{
		position_q6(x, y);
	}
}

// Place an open popup and restore its size constraints.
void Popup::place_open_popup(const Area & area, bool & in_layout)
{
	Size original_size = m_size;
	Size original_min_size = m_min_size;
	Size original_max_size = m_max_size;
	Dim width = 0;
	Dim height = 0;
	fit_size_to_area(area, width, height);
	m_size.set_q6(width, height);
	position_popup(area, width, height);

	in_layout = false;
	Pane::place(area, in_layout);
	m_size = original_size;
	m_min_size = original_min_size;
	m_max_size = original_max_size;
}

Widget * Popup::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;
	if (m_open)
	{
		result = Pane::hovered(parent_region, position);
	}
	return result;
}