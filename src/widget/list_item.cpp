#include "berialdraw_imp.hpp"

using namespace berialdraw;

ListItem::ListItem(Widget* parent, Dim index) :
	Widget("list_item", parent, sizeof(ListItem), index)
{
	UIManager::styles()->apply(this, (CommonStyle  *)this);
	UIManager::styles()->apply(this, (WidgetStyle  *)this);
	UIManager::styles()->apply(this, (BorderStyle  *)this);
	UIManager::styles()->apply(this, (TextStyle    *)this);
	UIManager::styles()->apply(this, (ListItemStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle *)this);
	bind(this, &ListItem::on_key);
	bind(this, &ListItem::on_click);
}

ListItem::~ListItem()
{
}

void ListItem::copy(const ListItem& obj)
{
	*((CommonStyle  *)this) = *(CommonStyle  *)(&obj);
	*((WidgetStyle  *)this) = *(WidgetStyle  *)(&obj);
	*((BorderStyle  *)this) = *(BorderStyle  *)(&obj);
	*((TextStyle    *)this) = *(TextStyle    *)(&obj);
	*((ListItemStyle*)this) = *(ListItemStyle*)(&obj);
	*((PaddingStyle *)this) = *(PaddingStyle *)(&obj);
}

void ListItem::copy(const ListItem* list_item)
{
	if (list_item)
	{
		copy(*list_item);
	}
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
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void ListItem::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	ListItemStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode ListItem::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
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
	place_in_area_extend(area, in_layout);

	// Paint text
	if (m_text.size() > 0)
	{
		m_text_foreclip = m_contentclip;
		m_text_foreclip.position().move_q6(m_leading_size.width_q6(), 0);
		m_text_foreclip.size().decrease_q6(m_leading_size.width_q6() + m_trailing_size.width_q6(), 0);
		place_in_layout(m_text_foreclip, m_text_foreclip.size(), marg, EXTEND_NONE, m_text_foreclip, m_text_align);
	}

	// Paint leading
	if (m_leading.size() > 0)
	{
		m_leading_foreclip = m_contentclip;
		m_leading_foreclip.size().width_q6(m_leading_size.width_q6());
		place_in_layout(m_leading_foreclip, m_leading_size, marg, EXTEND_NONE, m_leading_foreclip, Align::ALIGN_DEFAULT);
	}

	// Paint trailing
	if (m_trailing.size() > 0)
	{
		m_trailing_foreclip = m_contentclip;
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
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		UIManager::renderer()->region(region);

		// Paint background and border
		uint32_t color;
		uint32_t text_color;
		bool focused = false;

		List * list = search_list();
		if (list)
		{
			focused = list->focused();
		}

		if (m_selected)
		{
			text_color = stated_color(m_selected_text_color, focused);
			color = stated_color(m_selected_color, focused);
		}
		else
		{
			text_color = stated_color(m_text_color, focused);
			color = stated_color(m_color, focused);
		}

		RectRenderer::paint_rect(m_foreclip, color);

		select_font();

		// Paint text
		paint_item_part(region, m_text, m_text_sketch, m_text_box, m_text_foreclip, text_color);

		// Paint leading
		paint_item_part(region, m_leading, m_leading_sketch, m_leading_box, m_leading_foreclip, text_color);

		// Paint trailing
		paint_item_part(region, m_trailing, m_trailing_sketch, m_trailing_box, m_trailing_foreclip, text_color);
	}
}

/** Get the widget hovered */
Widget* ListItem::hovered(const Region& parent_region, const Point& position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if (region.is_inside(position) != Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Call back on key */
void ListItem::on_key(Widget * widget, const KeyEvent & evt)
{
}

/** Call back on click */
void ListItem::on_click(Widget * widget, const ClickEvent & evt)
{
	List * list = search_list();
	if (list)
	{
		list->focused(true);
		if (list->selection_mode() == ListSelectionMode::LIST_MULTI_SELECTION)
		{
			m_selected = !m_selected;
		}
		else if (list->selection_mode() == ListSelectionMode::LIST_SINGLE_SELECTION)
		{
			list->unselect_all();
			m_selected = true;
		}
	}
}

// Paint a single item part (text, leading, or trailing)
void ListItem::paint_item_part(const Region& region, const String& content,
                              std::unique_ptr<Sketch>& sketch, std::unique_ptr<TextBox>& text_box,
                              const Area& foreclip, uint32_t text_color)
{
	if (content.size() > 0)
	{
		Point shift;
		Region item_region(region);
		item_region.intersect(foreclip);
		UIManager::renderer()->region(item_region);
		if (sketch)
		{
			sketch->paint(foreclip, text_color);
		}
		if (text_box)
		{
			text_box->paint(shift, *m_font.get(), content, foreclip.position(), foreclip, text_color, 0, 0, true);
		}
	}
}

/** Search parent list container */
List * ListItem::search_list()
{
	List * result = nullptr;
	Widget * current = parent();

	while(current)
	{
		result = dynamic_cast<List*>(current);
		if (result)
		{
			break;
		}
		current = current->parent();
	}
	return result;
}