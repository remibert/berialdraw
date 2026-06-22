#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget */
List::List(Widget * parent):
	ScrollableContent("list", parent, sizeof(List)),
	m_column(nullptr),
	m_selected(0xFFFF),
	m_items()
{
	// Apply styles
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (ListStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);

	// Set default scroll direction to vertical only
	m_scroll_direction = SCROLL_VERTICAL;

	// Create internal column container
	m_column = new Column(this);
	m_column->margin(0, 0, 0, 0);
}

/** Destroy widget */
List::~List()
{
	m_items.clear();
}

/** Create a list item button */
Button * List::create_item(const String & text, const char * icon, bool enabled)
{
	Button * button = new Button(m_column);
	//button->text(text);
	//button->color(Color::TRANSPARENT);
	//button->border_color(Color::TRANSPARENT);
	//button->thickness(0);
	//button->enabled(enabled);

	button->text(text);                            // Sets button text to the provided item
	button->text_align(Align::ALIGN_LEFT);         // Aligns text to the left
	button->border_color(Color::LIGHT_BLACK,64);   // Sets border color to light black
	button->borders(Borders::BOTTOM_BORDER);       // Draws border only on the bottom side
	button->margin(0,0,1,0);                       // Adds margin around the button
		
	button->thickness(1);                          // Sets border thickness to 1
	button->radius(0);                             // Sets corner radius to 0 (square corners)

	
	// Add icon if provided
	//if (icon && icon[0])
	//{
	//	Icon * icon_widget = new Icon(button);
	//	icon_widget->filename(icon);
	//	icon_widget->size(16, 16);
	//}
	
	return button;
}

/** Update item appearance based on enabled state */
void List::update_item_appearance(Button * button, bool enabled)
{
	if (button)
	{
		button->enabled(enabled);
		if (enabled)
		{
			button->text_color(text_color());
		}
		else
		{
			button->text_color(disabled_text_color());
		}
	}
}

/** Get button at index */
Button * List::get_button(uint16_t index) const
{
	if (index < m_items.size())
	{
		return m_items[index];
	}
	return nullptr;
}

/** Add an item to the list */
uint16_t List::add(const String & text, const char * icon, bool enabled)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	
	Button * button = create_item(text, icon, enabled);
	button->enabled(enabled);
	m_items.push_back(button);
	
	return (uint16_t)(m_items.size() - 1);
}

/** Insert an item at a specific index */
void List::insert(uint16_t index, const String & text, const char * icon, bool enabled)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	
	Button * button = create_item(text, icon, enabled);
	button->enabled(enabled);
	
	if (index < m_items.size())
	{
		m_items.insert(index, button);
	}
	else
	{
		m_items.push_back(button);
	}
}

/** Remove an item at a specific index */
void List::remove(uint16_t index)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	
	if (index < m_items.size())
	{
		Button * button = m_items[index];
		m_items.remove(index);
		
		// Update selected index
		if (m_selected == index)
		{
			m_selected = 0xFFFF;
		}
		else if (m_selected > index && m_selected != 0xFFFF)
		{
			m_selected--;
		}
		
		// Delete button (it will be removed from column automatically)
		delete button;
	}
}

/** Clear all items from the list */
void List::clear()
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	
	// Delete all buttons
	for (uint16_t i = 0; i < m_items.size(); i++)
	{
		delete m_items[i];
	}
	m_items.clear();
	
	if (m_column)
	{
		m_column->clear();
	}
	m_selected = 0xFFFF;
}

/** Get the number of items in the list */
uint16_t List::count() const
{
	return m_items.size();
}

/** Set the selected item index */
void List::select(uint16_t index)
{
	if (index != m_selected)
	{
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
		m_selected = index;
		
		// Update button states if needed
		Button * button = get_button(index);
		if (button && button->enabled())
		{
			button->pressed(true);
		}
	}
}

/** Enable or disable an item */
void List::set_enabled(uint16_t index, bool enabled)
{
	Button * button = get_button(index);
	if (button)
	{
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
		update_item_appearance(button, enabled);
	}
}

/** Check if an item is enabled */
bool List::is_enabled(uint16_t index) const
{
	Button * button = get_button(index);
	return button ? button->enabled() : false;
}

/** Get the text of an item */
String List::text(uint16_t index) const
{
	Button * button = get_button(index);
	return button ? button->text() : String();
}

/** Set the text of an item */
void List::set_text(uint16_t index, const String & text)
{
	Button * button = get_button(index);
	if (button)
	{
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
		button->text(text);
	}
}

/** Get the icon of an item */
String List::icon(uint16_t index) const
{
	Button * button = get_button(index);
	if (button)
	{
		// Look for first icon widget child
		// Note: We can't directly access button->m_children (protected)
		// For now, return empty as direct child traversal is protected
		return String();
	}
	return String();
}

/** Set the icon of an item */
void List::set_icon(uint16_t index, const char * icon)
{
	Button * button = get_button(index);
	if (button)
	{
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
		
		// Icon setting is deferred to button's own management
		// For now, this is simplified
	}
}

/** Serialize the content of widget into json */
void List::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	ScrollViewStyle::serialize(it);
	ScrollbarStyle::serialize(it);
	ListStyle::serialize(it);
	BorderStyle::serialize(it);
	TextStyle::serialize(it);
	
	it["selected"] = (int)m_selected;
}

/** Unserialize the content of widget from json */
void List::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	ScrollViewStyle::unserialize(it);
	ScrollbarStyle::unserialize(it);
	ListStyle::unserialize(it);
	BorderStyle::unserialize(it);
	TextStyle::unserialize(it);
	
	m_selected = (int)(it["selected"] | (int)m_selected);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

/** Get the style cascade mode for this widget */
StyleCascadeMode List::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Return the size of content without marges */
Size List::content_size()
{
	return ScrollableContent::content_size();
}

/** Paint on screen memory the content of this widget */
void List::paint(const Region & parent_region)
{
	ScrollableContent::paint(parent_region);
	paint_border();
	paint_separators();
}

/** Paint the border around the list */
void List::paint_border()
{
	// Border is handled by BorderStyle through Button items
}

/** Paint separators between items */
void List::paint_separators()
{
	// Separators are handled by individual items
}

/** Event handler for item clicks */
void List::on_item_click(Widget * widget, const ClickEvent & evt)
{
	Button * button = dynamic_cast<Button*>(widget);
	if (button && button->enabled())
	{
		// Find the index in m_items
	for (uint16_t i = 0; i < m_items.size(); i++)
		{
			if (m_items[i] == button)
			{
				select(i);
				break;
			}
		}
	}
}

/** Event handler for focus changes to skip disabled items */
void List::on_item_focus(Widget * widget, const FocusEvent & evt)
{
	// This event is mostly informational
	// Button's focus handling is sufficient
}
