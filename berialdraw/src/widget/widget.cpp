#include "berialdraw_imp.hpp"

using namespace berialdraw;

Widget::Widget(const char * classname, Widget * parent):
	m_classname(classname),
	m_parent(parent)
{
	UIManager::invalidator()->dirty(this);
	m_place     = 1;
	m_pressed   = 0;
	m_focused   = 0;
	m_focusable = 0;
	m_refocus   = 1;
	m_extend    = 0;
	m_size_policy = 0;

	// If parent exists
	if(parent)
	{
		// If parent has not yet a children
		if(parent->m_children == 0)
		{
			// Add current widget in the children list
			parent->m_children = this;
		}
		else
		{
			// Select the first children of parent
			Widget* current = parent->m_children;
			while (current)
			{
				// If the last children found
				if(current->m_next == 0)
				{
					// Add current children to the parent list
					current->m_next = this;
					break;
				}
				// Select next children
				current = current->m_next;
			}
		}
	}
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
}

Widget::~Widget()
{
	clear();
	// Unlink parent
	if(m_parent)
	{
		UIManager::invalidator()->dirty(m_parent);
		// If the parent has children
		if(m_parent->m_children)
		{
			// If the first children is the current widget
			if(m_parent->m_children == this)
			{
				// Change the first children
				m_parent->m_children = m_parent->m_children->m_next;
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

const Area & Widget::foreclip() const
{
	return m_foreclip;
}

const Area & Widget::backclip() const
{
	return m_backclip;
}

const char * Widget::classname() const
{
	return m_classname;
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

void Widget::paint(const Region & parent_region)
{
	Widget* child = m_children;
	while (child)
	{
		if(child->paintable(parent_region))
		{
			child->paint(parent_region);
		}
		child = child->next();
	}
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
ScrollView * Widget::scroll_view()
{
	ScrollView * result = 0;
	Widget * current = this;

	while(current)
	{
		if (strcmp(current->classname(),"scroll_view") == 0)
		{
			result = dynamic_cast<ScrollView*>(current);
			break;
		}
		current = current->m_parent;
	}
	return result;
}

void Widget::replace_children()
{
	Widget * child = m_children;
	while(child)
	{
		child->m_place = 1;
		child->dirty();
		child->replace_children();
		child = child->m_next;
	}
}

/** Clean all dirty flag in all */
void Widget::clean_all()
{
	Widget * child = m_children;
	m_place = 1;
	UIManager::invalidator()->clear(this);
	while(child)
	{
		child->clean_all();
		child = child->m_next;
	}
}

void Widget::replace_all()
{
	replace_children();
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
		place_absolutly(area.position(), content_size(), m_foreclip, m_min_size, m_max_size);
	}
}

Size Widget::content_size()
{
	Size result = size() ;
	Size size;
	if (result.is_width_undefined())
	{
		result.width_(0);
	}
	if (result.is_height_undefined())
	{
		result.height_(0);
	}

	Widget* child = m_children;
	while (child)
	{
		size = child->content_size();

		if(size.height_() > result.height_())
		{
			result.height_(size.height_());
		}
		if(size.width_() > result.width_())
		{
			result.width_(size.width_());
		}
		child = child->next();
	}
	return result;
}

Size Widget::marged_size()
{
	Size result = content_size();
	result.height_(result.height_()+ margin().bottom_() + margin().top_());
	result.width_(result.width_()  + margin().left_() + margin().right_());
	//bd_printf("marged size %s w:%d h:%d\n",m_classname, result.width_(), result.height_());
	return result;
}

/** Get the size of children */
Size Widget::children_size()
{
	Size result = size();

	// If size not defined, set null size
	if (result.is_width_undefined())
	{
		result.width_(0);
	}
	if (result.is_height_undefined())
	{
		result.height_(0);
	}

	// Compute the children with marged size
	Size size;
	Widget* child = m_children;
	while (child)
	{
		size = child->marged_size();

		// If the children height is greater than pane height
		if(size.height_() > result.height_())
		{
			// Keep the greater height
			result.height_(size.height_());
		}

		// If the children width is greater than pane width
		if(size.width_() > result.width_())
		{
			// Keep the greater width
			result.width_(size.width_());
		}
		child = child->next();
	}
	return result;
}

bool Widget::paintable(const Region & parent_region)
{
	if(parent_region.is_inside(m_backclip.position().x_(),m_backclip.position().y_(), m_backclip.size().width_(), m_backclip.size().height_(), false) != Region::OUT)
	{
		return true;
	}
	return false;
}

/** Compute the scroll area */
void Widget::space_occupied(Point & min_position, Point & max_position)
{

	Widget* child = m_children;
	while (child)
	{
		{
			Point min;
			Point max;
			if (child->position().is_x_undefined() == false)
			{
				min.move_(child->position().x_(),0);
			}

			if (child->position().is_y_undefined() == false)
			{
				min.move_(0,child->position().y_());
			}

			max = min;

			max.move_(child->marged_size().width_(),child->marged_size().height_());

			if (min.x_() < min_position.x_())
			{
				min_position.x_(min.x_());
			}

			if (min.y_() < min_position.y_())
			{
				min_position.y_(min.y_());
			}

			if (max.x_() > max_position.x_())
			{
				max_position.x_(max.x_());
			}

			if (max.y_() > max_position.y_())
			{
				max_position.y_(max.y_());
			}
		}

		child->space_occupied(min_position,max_position);
		child = child->next();
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
			child->focusables(lst);
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
			UIManager::invalidator()->dirty(current_focus);
			UIManager::invalidator()->dirty(new_focus);
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
		UIManager::invalidator()->dirty(widget);
		widget->m_focused = 0;
	}

	if (all.size() >= 1)
	{
		Widget * new_widget_focus = 0;
		
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
			UIManager::invalidator()->dirty(new_widget_focus);
			widget = new_widget_focus;
			new_widget_focus->m_focused = 1;

			ScrollView * scroll_view = dynamic_cast<ScrollView*>(widget->scroll_view());
			if (scroll_view)
			{
				scroll_view->scroll_focus(widget);
			}
		}
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
		UIManager::invalidator()->dirty(widget);
		widget->m_focused = 0;
	}
	if (all.size() >= 1)
	{
		Widget * new_widget_focus = 0;

		// Find the widget previous to the one with focus
		for (uint32_t i= (all.size()-1); i >= 0; i--)
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

		// If no previous widget was found
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

			// If no previous widget was found
			if (new_widget_focus == 0)
			{
				new_widget_focus = all[0];
			}
		}
		if (new_widget_focus)
		{
			UIManager::invalidator()->dirty(new_widget_focus);
			widget = new_widget_focus;
			new_widget_focus->m_focused = 1;
			ScrollView * scroll_view = dynamic_cast<ScrollView*>(widget->scroll_view());
			if (scroll_view)
			{
				scroll_view->scroll_focus(widget);
			}
		}
	}
	else
	{
		widget = 0;
	}
}

/** Get the widget hovered */
Widget * Widget::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;

	// Create region with parent
	Region region(parent_region);

	// Restrict the region with the current widget area
	region.intersect(m_backclip);

	// If the widget hovered
	if(region.is_inside(position))
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
	return result;
}

/** Indicates if the window must be refreshed */
bool Widget::dirty()
{
	bool result = false;

	if(	UIManager::invalidator()->is_dirty(this))
	{
		result = true;
	}
	else
	{
		Widget* child = m_children;
		while (child && result == false)
		{
			result = child->dirty();
			if (result)
			{
				break;
			}
			child = child->m_next;
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

