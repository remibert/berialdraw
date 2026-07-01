#include "berialdraw_imp.hpp"

using namespace berialdraw;

Widget::Widget(const char * classname, Widget * parent, size_t size_of_widget):
	m_classname(classname),
	m_parent(parent)
{
	m_pressed   = 0;
	m_focused   = 0;
	m_focusable = 0;
	m_refocus   = 1;
	m_extend    = 0;
	m_size_policy = 0;

	// If parent exists
	if(parent)
	{
		// Get the actual parent (may be different if parent redirects children)
		Widget * parent_for_child = parent->get_parent_for_child();
		if (parent_for_child)
		{
			parent = parent_for_child;
		}
		m_parent = parent;

		// If parent has not yet a children
		if(parent->m_children == 0)
		{
			// Add current widget in the children list
			parent->m_children = this;
			parent->m_last_children = this;
		}
		else
		{
			// Add to the last children directly using the cached pointer
			parent->m_last_children->m_next = this;
			parent->m_last_children = this;
		}
	}
	UIManager::invalidator()->add(this, size_of_widget);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}



/* Clears all children from the widget */
void Widget::clear()
{
	Widget* current = m_children;
	Widget* next;

	// Remove all children
	while (current)
	{
		next = current->m_next;
		delete current;
		current = next;
	}
	
	// Reset both pointers when clearing
	m_children = 0;
	m_last_children = 0;
}

Widget::~Widget()
{
	clear();

	if (UIManager::invalidator())
	{
		UIManager::invalidator()->remove(this);
	}

	if (m_focused)
	{
		Window * window = dynamic_cast<Window*>(Widget::root());
		if (window)
		{
			window->remove_focus(this);
		}
	}

	// Unlink parent
	if(m_parent)
	{
		if (UIManager::invalidator())
		{
			UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
		}
		// If the parent has children
		if(m_parent->m_children)
		{
			// If the first children is the current widget
			if(m_parent->m_children == this)
			{
				// Change the first children
				m_parent->m_children = m_parent->m_children->m_next;
				// If no more children, reset m_last_children
				if(m_parent->m_children == 0)
				{
					m_parent->m_last_children = 0;
				}
			}
			else
			{
				// Search the current widget in children
				Widget* child = m_parent->m_children;
				while (child)
				{
					// If current widget found
					if (child->m_next == this)
					{
						// Remove current widget from the list
						child->m_next = child->m_next->m_next;
						
						// If this was the last child, update m_last_children to point to the previous child
						if(m_parent->m_last_children == this)
						{
							m_parent->m_last_children = child;
						}
						break;
					}

					// Select next widget
					child = child->m_next;
				}
			}
		}
	}


	if (UIManager::notifier())
	{
		UIManager::notifier()->remove(this);
	}
}

Widget * Widget::parent()
{
	return m_parent;
}

void Widget::apply_cascade_styles()
{
	// Apply style to this widget if parent is cascading
	Widget * style_source = m_parent;
	
	while (style_source && style_source->style_cascade_mode() == StyleCascadeMode::TRANSPARENT)
	{
		style_source = style_source->m_parent;
	}
	if (m_style_modified == 1)
	{
		// If we found a cascading parent with a style, apply it
		if (style_source && style_source->style_cascade_mode() == StyleCascadeMode::CASCADING)
		{
			const String & style_name = style_source->style();
			if (style_name.size() > 0)
			{
				// Use cached parsed properties to avoid re-parsing JSON for each cell
				Json * cached_properties = UIManager::styles()->get_style_properties(style_name.c_str());
				if (cached_properties)
				{
					JsonIterator it(*cached_properties);
					unserialize(it);
					UIManager::invalidator()->dirty(this, Invalidator::ALL);
				}
			}
		}
		m_style_modified = 0;
	}
	
	// Recursively apply styles to all children
	Widget * child = m_children;
	while (child)
	{
		child->apply_cascade_styles();
		child = child->m_next;
	}
}

const Area & Widget::foreclip() const
{
	return m_foreclip;
}

const Area & Widget::backclip() const
{
	return m_backclip;
}

void Widget::place(const Area & area, bool in_layout)
{
	m_backclip = area;
	Widget* child = m_children;
	while (child)
	{
		child->place(area, in_layout);
		child = child->m_next;
	}
}

void Widget::scroll(const Point & move)
{
	Widget* child = m_children;
	while (child)
	{
		child->m_backclip.move(move);
		child->m_foreclip.move(move);
		child->scroll(move);
		child = child->m_next;
	}
}

void Widget::paint(const Region & parent_region)
{
	// Get the bounding box of the parent region for fast culling
	Area bounds = parent_region.get_extents();
	
	Widget* child = m_children;
	while (child)
	{
		// Fast check: skip if child is completely outside the parent region bounds
		if (!child->m_backclip.is_outside(bounds))
		{
			if (child->hidden() == false)
			{
				child->paint(parent_region);
			}
		}
		child = child->next();
	}
}

/** Return the focusable widget parent of this widget */
Widget * Widget::focusable_parent()
{
	Widget * result = 0;
	Widget * parent = this;
	while(parent->m_parent)
	{
		parent = parent->m_parent;
		if (parent->m_focusable)
		{
			result = parent;
		}
	}
	return result;
}

Widget * Widget::root()
{
	Widget * parent = this;
	while(parent->m_parent)
	{
		parent = parent->m_parent;
	}
	return parent;
}

/** Return the scroll_view parent of this widget or null if not found */
Widget * Widget::scrollable_content()
{
	Widget * result = 0;
	Widget * current = this;

	while(current)
	{
		result = dynamic_cast<ScrollView*>(current);
		if(result)
		{
			break;
		}
		result = dynamic_cast<TableView*>(current);
		if(result)
		{
			break;
		}

		result = dynamic_cast<List*>(current);
		if(result)
		{
			break;
		}
		current = current->m_parent;
	}
	return result;
}

/** Clean all dirty flag in all */
void Widget::clean_all()
{
	UIManager::invalidator()->window_clean_all(root());
}

Area Widget::area()
{
	return m_backclip;
}

void Widget::place_in_area(const Area & area, bool in_layout)
{
	m_backclip = area;
	
	if(in_layout)
	{
		place_in_layout(area, content_size(), margin(), extend(), m_foreclip, align());
	}
	else
	{
		place_absolutly(area.position(), content_size(), m_foreclip, m_size, m_min_size, m_max_size);
	}
}

/** Indicates whether the widget is positioned absolutely */
bool Widget::is_absolute()
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		return false;
	}
	return true;
}

/** Return the size of children content without marges */
Size Widget::children_content_size()
{
	Size result;
	Size size;

	Widget* child = m_children;
	while (child)
	{
		size = child->content_size();

		if (size.height_q6() > result.height_q6())
		{
			result.height_q6(size.height_q6());
		}
		if (size.width_q6() > result.width_q6())
		{
			result.width_q6(size.width_q6());
		}
		child = child->next();
	}
	return result;
}

Size Widget::content_size()
{
	Size result = children_content_size();

	if (size().is_width_defined())
	{
		if (size().width_q6() > result.width_q6())
		{
			result.width_q6(size().width_q6());
		}
	}

	if (size().is_height_defined())
	{
		if (size().height_q6() > result.height_q6())
		{
			result.height_q6(size().height_q6());
		}
	}

	return result;
}

Size Widget::marged_size()
{
	Size result = content_size();
	result.height_q6(result.height_q6()+ margin().bottom_q6() + margin().top_q6());
	result.width_q6(result.width_q6()  + margin().left_q6() + margin().right_q6());
	//result.print(String("\nmarged size ")+ m_classname);
	return result;
}

/** Get the size of children */
Size Widget::children_size()
{
	Size result = size();

	// If size not defined, set null size
	if (result.is_width_undefined())
	{
		result.width_q6(0);
	}
	if (result.is_height_undefined())
	{
		result.height_q6(0);
	}

	// Compute the children with marged size
	Size size;
	Widget* child = m_children;
	while (child)
	{
		size = child->marged_size();

		// If the children height is greater than pane height
		if(size.height_q6() > result.height_q6())
		{
			// Keep the greater height
			result.height_q6(size.height_q6());
		}

		// If the children width is greater than pane width
		if(size.width_q6() > result.width_q6())
		{
			// Keep the greater width
			result.width_q6(size.width_q6());
		}
		child = child->next();
	}
	return result;
}


/** Add dirty on the widget and all of its children */
void Widget::dirty_children(enum Invalidator::Status status)
{
	Widget* child = m_children;
	UIManager::invalidator()->dirty(this, status);
	if (status & Invalidator::GEOMETRY)
	{
		m_geometry_modified = 1;
	}

	while (child)
	{
		child->dirty_children(status);
		child = child->next();
	}
}

/** Compute the size according to different size defined */
Size Widget::compute_size(const Size & size, const Size & min_size, const Size & max_size, const Margin & margin)
{
	Size result;

	if (!size.is_height_undefined())
	{
		result.height_q6(size.height_q6());
	}

	if (!size.is_width_undefined())
	{
		result.width_q6(size.width_q6());
	}

	if (!min_size.is_height_undefined())
	{
		if (result.height_q6() < min_size.height_q6())
		{
			result.height_q6(min_size.height_q6());
		}
	}

	if (!min_size.is_width_undefined())
	{
		if (result.width_q6() < min_size.width_q6())
		{
			result.width_q6(min_size.width_q6());
		}
	}

	if (!max_size.is_height_undefined())
	{
		if (result.height_q6() > max_size.height_q6())
		{
			result.height_q6(max_size.height_q6());
		}
	}

	if (!max_size.is_width_undefined())
	{
		if (result.width_q6() > max_size.width_q6())
		{
			result.width_q6(max_size.width_q6());
		}
	}

	if (result.is_height_undefined() == false || result.is_width_undefined() == false)
	{
		result.increase_q6(margin.left_q6() + margin.right_q6(), margin.top_q6() + margin.bottom_q6());
	}
	return result;
}

/** Compute the scroll area */
void Widget::one_space_occupied(Point & min_position, Point & max_position, const Point & position, const Size & marged)
{
	Point min;
	Point max;
	if (position.is_x_undefined() == false)
	{
		min.move_q6(position.x_q6(),0);
	}

	if (position.is_y_undefined() == false)
	{
		min.move_q6(0,position.y_q6());
	}

	max = min;

	max.move_q6(marged.width_q6(),marged.height_q6());

	if (min.x_q6() < min_position.x_q6())
	{
		min_position.x_q6(min.x_q6());
	}

	if (min.y_q6() < min_position.y_q6())
	{
		min_position.y_q6(min.y_q6());
	}

	if (max.x_q6() > max_position.x_q6())
	{
		max_position.x_q6(max.x_q6());
	}

	if (max.y_q6() > max_position.y_q6())
	{
		max_position.y_q6(max.y_q6());
	}
}


/** Compute the scroll area */
void Widget::space_occupied(Point & min_position, Point & max_position)
{
	Widget* child = m_children;
	if (child)
	{
		Size marged = child->content_size();
		while (child)
		{
			Point pos = child->position();
			one_space_occupied(min_position, max_position, pos, marged);
			child->space_occupied(min_position, max_position);
			child = child->next();
		}
	}
}

/** Return the color value when the widget is pressed */
uint32_t Widget::pressed_color(uint32_t color, bool pressed)
{
	uint32_t result = color;
	if (pressed)
	{
		// If color is transparent
		if (color == Color::TRANSPARENT)
		{
			// Search parent color
			Widget * widget = this;

			// Search the first color not transparent in widget parent
			do
			{
				widget = widget->parent();
				if (widget)
				{
					uint32_t parent_color = UIManager::colors()->color(widget->color());

					// Check if parent color not transparent
					if ((parent_color & Color::TRANSPARENT_MASK) == Color::TRANSPARENT_MASK)
					{
						color = parent_color;
						break;
					}
				}
			}
			while (widget);
		}

		// If dark color
		if ((Hsl::to_gray(color) & 0xFF) < 0x76)
		{
			// Enlight the color
			result = Hsl::add_color(color, Color::WHITE, 40);
		}
		else
		{
			// Darken the color
			result = Hsl::add_color(color, Color::BLACK, 10);
		}
	}
	return result;
}

/** Return the state color */
uint32_t Widget::stated_color(uint32_t color)
{
	uint32_t result = color;
	result = UIManager::colors()->color(color, m_focused == 1);
	result = pressed_color(result, m_pressed);
	return result;
}


/** Return the parent focus color */
uint32_t Widget::parent_focus_color(uint32_t color)
{
	uint32_t result = color;

	// If focus color inherited from focusable parent
	if (m_inherited_focus_color)
	{
		Widget * parent = focusable_parent();
	
		if (parent)
		{
			TextStyle * style = dynamic_cast<TextStyle*>(parent);
			if (style)
			{
				result = parent->stated_color(style->text_color());
			}				
		}
	}

	return result;
}




Widget * Widget::search(uint16_t id)
{
	Widget * result = 0;

	if(id == this->id())
	{
		result = this;
	}
	else
	{
		Widget* child = m_children;
		while (child)
		{
			result = child->search(id);
			if(result)
			{
				break;
			}
			child = child->m_next;
		}
	}
	return result;
}

void Widget::focusables(Vector<Widget *> & lst)
{
	if (m_focusable == 0)
	{
		Widget * child = m_children;
		while (child)
		{
			// Ignore hidden widget, in this case it cannot receive focus
			if (child->hidden() == false)
			{
				child->focusables(lst);
			}
			child = child->m_next;
		}
	}
	else
	{
		lst.push_back(this);
	}
}

/** Set the focus on this widget */
void Widget::focus_to(Widget * & current_focus, Widget * new_focus)
{
	if (new_focus)
	{
		if (new_focus->m_focusable)
		{
			if (current_focus)
			{
				current_focus->m_focused = 0;
			}
			UIManager::invalidator()->dirty(current_focus, Invalidator::REDRAW);
			UIManager::notifier()->push_event(new FocusEvent(false, current_focus));

			UIManager::invalidator()->dirty(new_focus, Invalidator::REDRAW);
			UIManager::notifier()->push_event(new FocusEvent(true, new_focus));
			current_focus = new_focus;
			
			new_focus->m_focused = 1;
		}
	}
}

void Widget::focus_next(Widget * & widget)
{
	Vector<Widget *> all;
	focusables(all);

	if (widget)
	{
		UIManager::invalidator()->dirty(widget, Invalidator::REDRAW);
		UIManager::notifier()->push_event(new FocusEvent(false, widget));
		widget->m_focused = 0;
	}

	if (all.size() >= 1)
	{
		Widget * new_widget_focus = 0;
		bool found = false;
		
		// Find the widget next to the one with focus
		for (uint32_t i= 0; i < all.size(); i++)
		{
			if (all[i] == widget)
			{
				if (i == (all.size() -1))
				{
					new_widget_focus = all[0];
				}
				else
				{
					new_widget_focus = all[i+1];
				}
				break;
			}
		}

		change_focus(widget, new_widget_focus, all);
	}
	else
	{
		widget = 0;
	}
}

void Widget::focus_previous(Widget * & widget)
{
	Vector<Widget *> all;
	focusables(all);
	if (widget)
	{
		UIManager::invalidator()->dirty(widget, Invalidator::REDRAW);
		UIManager::notifier()->push_event(new FocusEvent(false, widget));
		widget->m_focused = 0;
	}
	if (all.size() >= 1)
	{
		Widget * new_widget_focus = 0;

		// Find the widget previous to the one with focus
		for (int32_t i= (all.size()-1); i >= 0; i--)
		{
			if (all[i] == widget)
			{
				if (i == 0)
				{
					new_widget_focus = all[all.size()-1];
				}
				else
				{
					new_widget_focus = all[i-1];
				}
				break;
			}
		}

		change_focus(widget, new_widget_focus, all);
	}
	else
	{
		widget = 0;
	}
}


void Widget::change_focus(Widget * & widget, Widget * & new_widget_focus, Vector<Widget *> & all)
{
	// If no widget widget was found
	if (new_widget_focus == 0)
	{
		// If no widget has yet received focus
		if (widget == 0)
		{
			// Checks if a widget has focus
			for (uint32_t i= 0; i < all.size(); i++)
			{
				// If a widget has focus
				if (all[i]->focused())
				{
					// If a widget with focus has not already been found
					if (widget == 0)
					{
						widget = all[i];
						new_widget_focus = all[i];
					}
					else
					{
						// Remove focus
						all[i]->focused(false);
					}
				}
			}
		}

		// If no next widget was found
		if (new_widget_focus == 0)
		{
			new_widget_focus = all[0];
		}
	}

	if (new_widget_focus)
	{
		UIManager::invalidator()->dirty(new_widget_focus, Invalidator::REDRAW);
		UIManager::notifier()->push_event(new FocusEvent(true, new_widget_focus));
		widget = new_widget_focus;
		new_widget_focus->m_focused = 1;

		ScrollView * scroll_view = dynamic_cast<ScrollView*>(widget->scrollable_content());
		if (scroll_view)
		{
			scroll_view->scroll_focus(widget);
		}
		else
		{
			List * list = dynamic_cast<List*>(widget->scrollable_content());
			if (list)
			{
				list->scroll_focus(widget);
			}
		}
	}
}


/** Get the widget hovered */
Widget * Widget::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;

	if (hidden() == false)
	{
		// Create region with parent
		Region region(parent_region);

		// Restrict the region with the current widget area
		region.intersect(m_backclip);

		// If the widget hovered
		if(region.is_inside(position) != Region::Overlap::OUT)
		{
			Widget * result_child = 0;
			Widget* child = m_children;

			result = this;

			// Scan all widget children
			while (child && result_child == 0)
			{
				result_child = child->hovered(region, position);
				child = child->next();
			}

			// If children hovered
			if(result_child)
			{
				// Select children
				result = result_child;
			}
		}
	}
	return result;
}

/** Serialize the content of widget into json */
void Widget::serialize(JsonIterator & it)
{
	int index = 0;
	Widget* child = m_children;
	JsonIterator children = it["children"];
	while (child)
	{
		JsonIterator widget   = children[index];
		child->serialize(widget);
		index ++;
		child = child->m_next;
	}
}


