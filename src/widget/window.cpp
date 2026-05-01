#include "berialdraw_imp.hpp"

using namespace berialdraw;

Window::Window() : Widget("window",0, sizeof(Window))
{
	m_allocated = false;
	UIManager::desktop()->add(this);

	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);

	bind(this, &Window::on_click_widget);
	bind(this, &Window::on_key_focus);
	bind(this, &Window::on_select_widget);
	m_refocus = true;
}

void* Window::operator new(size_t size)
{
	void* obj = ::operator new(size);
	static_cast<Window*>(obj)->m_allocated = true;
	return obj;
}

Window::~Window()
{
	if (UIManager::is_initialized())
	{
		UIManager::desktop()->remove(this);
	}
}

/** Copy all styles of the window */
void Window::copy(const Window & window)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&window);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&window);
}

/** Copy all styles of the window */
void Window::copy(const Window * window)
{
	if (window)
	{
		copy(*window);
	}
}

bool Window::is_allocated()
{
	return m_allocated;
}

/** Return the size of content without marges */
Size Window::content_size()
{
	Size result;

	// If the width defined
	if (m_size.is_width_undefined() == false)
	{
		// Set the width specified
		result.width_(m_size.width_());
	}
	else
	{
		// Set the width of screen
		result.width(UIManager::renderer()->size().width());
	}

	// If the height defined
	if (m_size.is_height_undefined() == false)
	{
		// Set the height specified
		result.height_(m_size.height_());
	}
	else
	{
		// Set the height of screen
		result.height(UIManager::renderer()->size().height());
	}

	return result;
}


/** Remove the focus of the widget */
void Window::remove_focus(Widget * widget)
{
	if (m_with_focus == widget)
	{
		focus_next(m_with_focus);
		if (m_with_focus == widget)
		{
			m_with_focus = 0;
			m_refocus = 1;
		}
	}
}


void Window::place()
{
	if (m_refocus)
	{
		m_refocus = 0;
		focus_next(m_with_focus);
	}
	
	Area oldForeclip(m_foreclip);

	// If the position or size not specified
	if (m_position.is_x_undefined() && m_position.is_y_undefined() || 
		m_size.is_width_undefined() && m_size.is_height_undefined())
	{
		// Place into screen area
		Area screen_area;
		screen_area.position().set(0,0);
		screen_area.size().set(UIManager::renderer()->size().width(),UIManager::renderer()->size().height());
		place_in_layout(screen_area, content_size(), margin(), extend(), m_foreclip, align());
	}
	else
	{
		// Set fixed position and size
		m_foreclip.set(m_position,m_size);
	}
	
	Widget::place(m_foreclip, false);

	// If size of window changed
	if (m_foreclip.width() != oldForeclip.width() || m_foreclip.height() != oldForeclip.height())
	{
		if (m_flow_in_children)
		{
			// Force the flow placement
			m_flow_place = 1;
		}
	}
}

bool Window::flow_replacement()
{
	bool result = m_flow_place;
	m_flow_place = 0;
	return result;
}

void Window::paint(const Region & parent_region)
{
	// Create region of window
	Region region(m_backclip);

	// Remove other windows region
	region.subtract(parent_region);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		// Register region
		UIManager::renderer()->region(region);

		// Open svg export if it required
		Exporter * exporter = UIManager::exporter();

		// If export svg required
		if(exporter)
		{
			// Open svg group for the current window
			exporter->open_group(m_backclip.position(), m_backclip.size());
		}

		// Show window background
		Point shift;
		Rect rect(0);
			rect.size(m_backclip.size());
			rect.position(m_backclip.position());
			rect.radius_(0);
			rect.color(stated_color(m_color));
			rect.paint(shift);

		// Paint widgets children
		Widget::paint(region);

		// If svg export required
		if(exporter)
		{
			// Close svg group for the current window
			exporter->close_group();
		}
	}
}

/** Get the widget hovered */
Widget * Window::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;

	// Create region with window area
	Region region(m_backclip);

	// Remove the other window at foreground from the region
	region.subtract(parent_region);

	// If the touch is in the current window
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		Widget* child = m_children;
		Widget * hovered = 0;
		Widget * found = 0;

		result = this;

		// Scan all widget children
		while (child)
		{
			found = child->hovered(region, position);
			if (found)
			{
				hovered = found;
			}
			child = child->next();
		}

		// If children hovered
		if(hovered)
		{
			// Select children
			result = hovered;
		}
	}
	return result;
}

/** Places the specified window at the top of its parents canvas stack, above all of its sibling window. */
void Window::back()
{
	UIManager::desktop()->back(this);
}

/** Places the specified window at the bottom of its parents canvas stack, below all of its sibling window.*/
void Window::front()
{
	UIManager::desktop()->front(this);
}

/** Serialize the content of widget into json */
void Window::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	Widget::serialize(it);
}

/** Unserialize the content of widget from json */
void Window::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
}

/** Force flow replacement */
void Window::force_flow_replacement()
{
	m_flow_in_children = 1;
	m_flow_place = 1;
}

/** Treat the key to change the focused widget */
void Window::on_key_focus(Widget * widget, const KeyEvent & evt)
{
	if (evt.state() == KeyEvent::KEY_DOWN)
	{
		switch(evt.key())
		{
		case (wchar_t)ReservedKey::KEY_TABULATION  :
			if (evt.modifier() & KeyEvent::MODIFIER_SHIFT)
			{
				focus_previous(m_with_focus);
			}
			else
			{
				focus_next(m_with_focus);
			}
			break;
		}
	}
}

/** Treat the click on widget, used to change the focus */
void Window::on_click_widget(Widget * widget, const ClickEvent & evt)
{
	focus_to(m_with_focus, widget);
}

/** Treat the select widget, used to change the focus */
void Window::on_select_widget(Widget * widget, const SelectEvent & evt)
{
	focus_to(m_with_focus, widget);
}


