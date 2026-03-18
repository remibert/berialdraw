#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** ScrollableContent Implementation */

ScrollableContent::ScrollableContent(const char * classname, Widget * parent, size_t size_of_widget):
	Widget(classname, parent, size_of_widget)
{
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (ScrollViewStyle*)this);
	m_color = Color::TRANSPARENT;
	m_size.clean();
	bind(this, &ScrollableContent::on_scroll);
}

ScrollableContent::~ScrollableContent()
{
}

/** Call back on scroll */
void ScrollableContent::on_scroll(Widget * widget, const ScrollEvent & evt)
{
	m_scroll_position.move(evt.shift());
	UIManager::invalidator()->dirty(this,Invalidator::GEOMETRY);
}

/** Compute the scroll area */
void ScrollableContent::space_occupied(Point & min_position, Point & max_position)
{
	Widget::space_occupied(min_position,max_position);
}

Coord ScrollableContent::calc_shift_focus(Coord widget_position, Dim widget_size, Coord viewport_position, Dim viewport_size)
{
	Coord result = 0;

	// shift to center widget in view
	Coord shift = ((Coord)viewport_size - (Coord)widget_size) / 2;

	// If shift is greater than the widget
	if (shift > (Coord)widget_size)
	{
		shift = widget_size;
	}
	
	// If the focused widget after view port
	if ((widget_position + (Coord)widget_size) > (viewport_position + (Coord)viewport_size))
	{
		result -= (viewport_position + (Coord)viewport_size) - (widget_position + (Coord)widget_size) -  shift;
	}
	// If the focused widget is before view port
	else if (widget_position < viewport_position)
	{
		result -= viewport_position - widget_position + shift;
	}
	return result;
}

Coord ScrollableContent::compute_scroll(Coord & scroll_position, Coord & m_scroll_position, Dim scroll_size, Dim area, Align align)
{
	Coord result = 0;

	// If the view is smaller than the content size
	if (area <= scroll_size)
	{
		Coord delta = (Coord)(scroll_size - area);

		// If the scroll is before the content
		if (scroll_position < 0)
		{
			result = scroll_position;
			scroll_position = 0;
			m_scroll_position = 0;
		}
		// If the scroll is after the content
		else if (scroll_position > delta)
		{
			result = scroll_position-delta;
			scroll_position = delta;
			m_scroll_position = delta;
		}
		scroll_position = -scroll_position;
	}
	// If the view is greater than the content size
	else
	{
		Coord delta = (Coord)(area - scroll_size);
		
		// If the scrolled content must be aligned to the left
		if ((align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
		{
			result = scroll_position;
			scroll_position = 0;
			m_scroll_position = 0;
		}
		// If the scrolled content must be aligned to the right
		else if ((align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_RIGHT)
		{
			result = scroll_position-delta;
			scroll_position = delta;
			m_scroll_position = delta;
		}
		// the scrolled content is centered
		else
		{
			result = scroll_position-(delta/2);
			scroll_position = delta/2;
			m_scroll_position = delta/2;
		}
	}
	return result;
}

Point ScrollableContent::compute_scroll_view(const Area & area, Point & scroll_position, Size & scroll_size)
{
	Point result;
	Area fixed_area;

	// If scroll size not specified
	if (m_scroll_size.is_width_undefined() || m_scroll_size.is_height_undefined())
	{
		m_size = Widget::content_size();

		// Compute the absolute positionned widget area
		scroll_area(fixed_area);

		if (m_size.width_() < fixed_area.width_())
		{
			m_size.width_(fixed_area.width_());
		}

		if (m_size.height_() < fixed_area.height_())
		{
			m_size.height_(fixed_area.height_());
		}

		// If the scrolled content can extend only to the width (vertical scroll)
		if (m_scroll_direction == SCROLL_VERTICAL)
		{
			// Reduce the scrolled content width to the viewport width
			m_size.width(area.width());
		}
		// If the scrolled content can extend only to the height (horizontal scroll)
		else if (m_scroll_direction == SCROLL_HORIZONTAL)
		{
			// Reduce the scrolled content height to the viewport height
			m_size.height(area.height());
		}
		scroll_size = m_size;
	}

	Coord scroll_position_x   = scroll_position.x();
	Coord scroll_position_y   = scroll_position.y();
	Coord m_scroll_position_x = m_scroll_position.x();
	Coord m_scroll_position_y = m_scroll_position.y();
	Coord result_x;
	Coord result_y;
	result_x = compute_scroll(scroll_position_x, m_scroll_position_x, scroll_size.width(),  area.width(),  (Align)m_align);
	result_y = compute_scroll(scroll_position_y, m_scroll_position_y, scroll_size.height(), area.height(), (Align)(m_align>>2));
	result.set(result_x, result_y);

	scroll_position_x -= fixed_area.position().x();
	scroll_position_y -= fixed_area.position().y();

	scroll_position.set(scroll_position_x, scroll_position_y);
	m_scroll_position.set(m_scroll_position_x, m_scroll_position_y);

	return result;
}

void ScrollableContent::place(const Area & area, bool in_layout)
{
	Area scroll_area;
	Size scroll_size(m_scroll_size);
	Point previous_position(m_scroll_position);
	Point scroll_position(m_scroll_position);
	Area marged_area(area);

	marged_area.decrease(margin());

	// Compute the scroll position and size
	Point scroll_out = compute_scroll_view(marged_area, scroll_position, scroll_size);

	if (scroll_out.x() | scroll_out.y())
	{
		if (m_parent)
		{
			UIManager::notifier()->scroll(scroll_out.x(), scroll_out.y(), m_parent->scrollable_content());
		}
	}

	// Move to the screen position
	scroll_position.move(marged_area.position());

	// Set the scroll area to calculate the position of all widgets in the scrolled content
	scroll_area.set(scroll_position, scroll_size);

	// Place all children in scrolled area
	Widget::place(scroll_area, false);
	
	// Define the visible area (clipping area) for this scrollable widget
	m_foreclip = area;
	m_backclip = marged_area;
}

/** Paint on screen memory the content of this widget */
void ScrollableContent::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		Exporter * exporter = UIManager::exporter();

		if (exporter)
		{
			exporter->open_group(m_backclip.position(), m_backclip.size());
		}

		Widget::paint(region);

		if (exporter)
		{
			exporter->close_group();
		}
	}
}

/** Get scroll size */
const Size & ScrollableContent::scroll_size() const
{
	if (m_scroll_size.is_width_undefined() && m_scroll_size.is_height_undefined())
	{
		return m_size;
	}
	else
	{
		return m_scroll_size;
	}
}

/** Set scroll size */
void ScrollableContent::scroll_size(const Size & size)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_size = size;
}

/** Set the scroll size with width and height in pixels */
void ScrollableContent::scroll_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_size.set(w,h);
}

/** Set scroll position */
void ScrollableContent::scroll_position(const Point & position)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_position = position;
}

/** Set the scroll position with x and y in pixels */
void ScrollableContent::scroll_position(Coord x, Coord y)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_scroll_position.set(x,y);
}

/** Get scroll position */
const Point & ScrollableContent::scroll_position() const
{
	return m_scroll_position;
}

/** Get viewport size */
const Size & ScrollableContent::viewport_size() const
{
	if (m_viewport_size.is_width_undefined() && m_viewport_size.is_height_undefined())
	{
		return m_backclip.size();
	}
	else
	{
		return m_viewport_size;
	}
}

/** Set viewport size */
void ScrollableContent::viewport_size(const Size & size)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_viewport_size = size;
}

/** Set the viewport size with width and height in pixels */
void ScrollableContent::viewport_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_viewport_size.set(w,h);
}

/** Return the size of content without marges */
Size ScrollableContent::content_size()
{
	Size result = Widget::content_size();
	if (!m_viewport_size.is_width_undefined())
	{
		result.width_(m_viewport_size.width_());
	}
	if (!m_viewport_size.is_height_undefined())
	{
		result.height_(m_viewport_size.height_());
	}
	return result;
}

/** Move the scroll view to see the widget with the focus */
void ScrollableContent::scroll_focus(Widget * widget)
{
	if (widget)
	{
		Coord x = calc_shift_focus(widget->backclip().x(), widget->backclip().width(),  m_backclip.x(), m_backclip.width());
		Coord y = calc_shift_focus(widget->backclip().y(), widget->backclip().height(), m_backclip.y(), m_backclip.height());
		if (x | y)
		{
			UIManager::notifier()->scroll(x, y, this);
		}
	}
}

/** Compute the scroll area */
void ScrollableContent::scroll_area(Area & area)
{
	Point min_position;
	Point max_position;

	space_occupied(min_position,max_position);

	area.position().set_(min_position.x_(), min_position.y_());
	area.size().set_(max_position.x_()-min_position.x_(), max_position.y_()-min_position.y_());
}
