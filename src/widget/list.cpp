#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget */
List::List(Widget * parent):
	ScrollableContent("list", parent, sizeof(List)),
	m_column(nullptr)
{
	// Apply styles
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (ListStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);

	// Set default scroll direction to vertical only
	m_scroll_direction = SCROLL_VERTICAL;
	align(Align::ALIGN_TOP);

	// Create internal column container
	m_column = new Column(this);
	m_column->margin(0, 0, 0, 0);
	m_column->size_policy(SizePolicy::ENLARGE_WIDTH);
}

/** Destroy widget */
List::~List()
{
}

/** Create new list item */
ListItem* List::new_item()
{
	return new ListItem(m_column);
}


///** Add an item to the list */
//uint16_t List::add(const String & text)
//{
//	uint16_t result = 0;
//	add(text, "","");
//	return result;
//}

//void insert(Row * row, Button * button, const String & text, bool middle)
//{
//	if (text.size() > 0)
//	{
//		if (FileTools::match_pattern("*.icn",text,true))
//		{
//			Picture * icon = new Picture(row);
//				icon->size(button->font_size());
//				icon->filename(text);
//				icon->align(Align::ALIGN_LEFT);
//				icon->inherited_focus_color(true);
//				icon->extend(Extend::EXTEND_WIDTH);
//
//		}
//		else
//		{
//			Label * label = new Label(row);
//				label->text(text);
//				label->extend(Extend::EXTEND_WIDTH);
//				label->padding(5);
//				if (middle)
//				{
//					label->align(Align::ALIGN_LEFT);
//					label->size_policy(SizePolicy::ENLARGE_WIDTH);
//				}
//				else
//				{
//					label->size_policy(SizePolicy::SHRINK_WIDTH);
//				}
//				label->font_size(button->font_size());
//				label->inherited_focus_color(true);
//		}
//	}
//}

///** Add an item to the list */
//uint16_t List::add(const String & text, const String & leading, const String & trailing)
//{
//	uint16_t result = 0;
//
//	Button * button = new Button(m_column);
//		button->borders(Borders::BOTTOM_BORDER);  
//		button->borders(Borders::NO_BORDER);
//		button->margin(1,2,1,5);
//		button->thickness(1);
//		button->radius(0);
//		button->size_policy(SizePolicy::ENLARGE_WIDTH);
//		button->extend(Extend::EXTEND_WIDTH);
//		button->focus_thickness(0);
//		button->focus_color(Color::TRANSPARENT);
//
//		Row * row = new Row(button);
//			row->size_policy(SizePolicy::ENLARGE_WIDTH);
//			row->extend(Extend::EXTEND_WIDTH);
//			insert(row, button, leading, false);
//			insert(row, button, text, true);
//			insert(row, button, trailing, false);
//	return result;
//}

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


/** Place all widget in area */
void List::place(const Area& area, bool in_layout)
{
	Area foreground(area);
	foreground.decrease_thickness(m_thickness);
	//foreground.decrease(m_padding);
	ScrollableContent::place(area, in_layout);
}


/** Paint on screen memory the content of this widget */
void List::paint(const Region & parent_region)
{
	Region background_region(parent_region);

	// Draw rectangle
	background_region.intersect(m_backclip);

	// If list_item visible
	if (background_region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(background_region);

		Area foreground(m_backclip);
		foreground.decrease_thickness(m_thickness);

		// Paint background and border
		Rect::build_focused_polygon(foreground, *(CommonStyle*)this, *(BorderStyle*)this,
			stated_color(m_color), stated_color(m_border_color), Color::TRANSPARENT,
			stated_color(m_focus_color), m_focused);

		// Paint content
		ScrollableContent::paint(m_foreclip);
	}
}

