#include "berialdraw_imp.hpp"

using namespace berialdraw;

ListItem::ListItem(Widget* parent) :
	Widget("list_item", parent, sizeof(ListItem))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (ListItemStyle*)this);
}

ListItem::~ListItem()
{
}

void ListItem::copy(const ListItem& list_item)
{
	*((CommonStyle*)this)   = *(CommonStyle*)(&list_item);
	*((WidgetStyle*)this)   = *(WidgetStyle*)(&list_item);
	*((BorderStyle*)this)   = *(BorderStyle*)(&list_item);
	*((TextStyle*)this)     = *(TextStyle*)(&list_item);
	*((ListItemStyle*)this) = *(ListItemStyle*)(&list_item);
}

void ListItem::copy(const ListItem* list_item)
{
	if (list_item)
	{
		copy(*list_item);
	}
}

Size ListItem::select(String & text, std::unique_ptr<TextBox> & text_box, std::unique_ptr<Sketch> & sketch)
{
	Size result;
	if (FileTools::match_pattern("*.icn",text,true))
	{
		if (!sketch)
		{
			sketch = std::make_unique<Sketch>(this);
		}
		sketch->filename(text);
		sketch->load();
		sketch->size(font_size());
		result = font_size();
	}
	else
	{
		sketch.reset();
		Area area;

		if (!text_box)
		{
			text_box = std::make_unique<TextBox>();
		}
		text_box->parse(area, *m_font, text, UINT32_MAX, UINT32_MAX, UINT32_MAX, m_text_align);
		result = text_box->content_size();
	}
	return result;
}


Size ListItem::content_size()
{
	Size result;

	// If something modified
	if (m_text_modified || m_leading_modified || m_trailing_modified || m_font_modified)
	{
		Area area;
		select_font();
		if (m_font.get())
		{
			if (m_text_modified)
			{
				m_text_size = select(m_text,m_text_box,m_text_sketch);
			}

			if (m_leading_modified)
			{
				m_leading_size = select(m_leading,m_leading_box,m_leading_sketch);
			}

			if (m_trailing_modified)
			{
				m_trailing_size = select(m_trailing,m_trailing_box,m_trailing_sketch);
			}
		}
		m_text_modified = m_font_modified = m_trailing_modified = m_leading_modified = 0;
	}

	// Add the widths and set the greater height
	result.set_q6(
		m_leading_size.width_q6()+m_text_size.width_q6()+m_trailing_size.width_q6(),
		max(m_text_size.height_q6(),max(m_leading_size.height_q6(), m_trailing_size.height_q6())));

	// Add padding
	result.height_q6(result.height_q6() + padding().bottom_q6() + padding().top_q6());
	result.width_q6(result.width_q6()  + padding().left_q6()   + padding().right_q6());

	return result;
}


void ListItem::place(const Area& area, bool in_layout)
{
	Margin marg;

	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place the item area
	Area item_area(m_foreclip);

	// Remove padding
	item_area.decrease(padding());

	// Paint text
	if (m_text.size() > 0)
	{
		m_text_foreclip = item_area;
		m_text_foreclip.position().move_q6(m_leading_size.width_q6(), 0);
		m_text_foreclip.size().decrease_q6(m_leading_size.width_q6() + m_trailing_size.width_q6(), 0);
		place_in_layout(m_text_foreclip, m_text_foreclip.size(), marg, EXTEND_NONE, m_text_foreclip, m_text_align);
	}

	// Paint leading
	if (m_leading.size() > 0)
	{
		m_leading_foreclip = item_area;
		m_leading_foreclip.size().width_q6(m_leading_size.width_q6());
		place_in_layout(m_leading_foreclip, m_leading_size, marg, EXTEND_NONE, m_leading_foreclip, Align::ALIGN_DEFAULT);
	}

	// Paint trailing
	if (m_trailing.size() > 0)
	{
		m_trailing_foreclip = item_area;
		m_trailing_foreclip.position().move_q6(m_text_foreclip.width_q6() + m_leading_foreclip.width_q6(), 0);
		m_trailing_foreclip.size().width_q6(m_trailing_size.width_q6());
		place_in_layout(m_trailing_foreclip, m_trailing_size, marg, EXTEND_NONE, m_trailing_foreclip, Align::ALIGN_DEFAULT);
	}
}

void ListItem::paint(const Region& parent_region)
{
	Region region(parent_region);

	// Draw rectangle
	region.intersect(m_backclip);

	// If list_item visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Point shift;

		// Paint background and border
		Rect::build_focused_polygon(m_foreclip, *(CommonStyle*)this, *(BorderStyle*)this,
			stated_color(m_color), stated_color(m_border_color), Color::TRANSPARENT,
			stated_color(m_focus_color), m_focused);

		select_font();

		// Paint text
		if (m_text.size() > 0)
		{
			Region text_region(region);
			text_region.intersect(m_text_foreclip);
			UIManager::renderer()->region(text_region);
			if (m_text_sketch)
			{
				Margin margin;
				m_text_sketch->paint(m_text_foreclip, Margin(), stated_color(m_text_color));
			}
			if (m_text_box)
			{
				m_text_box->paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_foreclip, stated_color(m_text_color), 0, 0, true);
			}
		}

		// Paint leading
		if (m_leading.size() > 0)
		{
			Region leading_region(region);
			leading_region.intersect(m_leading_foreclip);
			UIManager::renderer()->region(leading_region);
			if (m_leading_sketch)
			{
				Margin margin;
				m_leading_sketch->paint(m_leading_foreclip, Margin(), stated_color(m_text_color));
			}
			if (m_leading_box)
			{
				m_leading_box->paint(shift, *m_font.get(), m_leading, m_leading_foreclip.position(), m_leading_foreclip, stated_color(m_text_color), 0, 0, true);
			}
		}

		// Paint trailing
		if (m_trailing.size() > 0)
		{
			Region trailing_region(region);
			trailing_region.intersect(m_trailing_foreclip);
			UIManager::renderer()->region(trailing_region);
			if (m_trailing_sketch)
			{
				Margin margin;
				m_trailing_sketch->paint(m_trailing_foreclip, Margin(), stated_color(m_text_color));
			}
			if (m_trailing_box)
			{
				m_trailing_box->paint(shift, *m_font.get(), m_trailing, m_trailing_foreclip.position(), m_trailing_foreclip, stated_color(m_text_color), 0, 0, true);
			}
		}
	}
}

/** Get the widget hovered */
Widget* ListItem::hovered(const Region& parent_region, const Point& position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if (region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void ListItem::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	ListItemStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void ListItem::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	ListItemStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode ListItem::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}



