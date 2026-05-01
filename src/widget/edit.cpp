#include "berialdraw_imp.hpp"

using namespace berialdraw;

Edit::Edit(Widget * parent):
	Widget("edit", parent, sizeof(Edit)),
	Entry(&m_text),
	m_text_box()
{
	m_position = m_position;
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (EditStyle*)this);
	bind(this, &Edit::on_key);
	bind(this, &Edit::on_select);
	bind(this, &Edit::on_click);
	m_edited = 1;
}

Edit::~Edit()
{
}

/** Copy all styles of the edit */
void Edit::copy(const Edit & edit)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&edit);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&edit);
	*((BorderStyle*)this) = *(BorderStyle*)(&edit);
	*((TextStyle*)this)   = *(TextStyle*)(&edit);
	*((EditStyle*)this)   = *(EditStyle*)(&edit);
}

/** Copy all styles of the edit */
void Edit::copy(const Edit * edit)
{
	if(edit)
	{
		copy(*edit);
	}
}

Size Edit::content_size()
{
	Size result;
	select_font();

	if (m_edited == 1)
	{
		m_edited = 0;
		select_all();
		entry_align((Align)m_text_align);
	}

	if(m_text_modified)
	{
		Size char_size = m_font->text_size(" ");

		// Adapt the size of text according to its size specified
		m_text_size.set_(char_size.width_() * m_max_columns, char_size.height_() * m_max_lines);
		m_text_modified = 0;
	}

	result = m_text_size;
	result.height_(result.height_()+ padding().bottom_() + padding().top_() + m_focus_gap +m_focus_gap);
	result.width_(result.width_()  + padding().left_()   + padding().right_() + m_focus_gap + m_focus_gap);

	return result;
}

void Edit::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place rectangle
	place_in_area(area, in_layout);

	// If absolute place
	if (in_layout == false)
	{
		Area backclip = m_foreclip;
		Margin marg;
		Size size(content_size());

		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			size.height_(m_foreclip.height_());
		}
		else
		{
			size.width_(m_foreclip.width_());
		}
		place_in_layout(backclip, size, marg, EXTEND_NONE, m_foreclip, (Align)m_align);
	}

	// Place text
	m_text_backclip = m_foreclip;
	m_text_backclip.decrease(padding());

	// If text area greater than the max columns authorised
	if (m_text_backclip.width_() > m_text_size.width_())
	{
		// Reduce size to the max columns
		m_text_backclip.width_(m_text_size.width_());
	}
	
	// If text area greater than the max lines authorised
	if (m_text_backclip.height_() > m_text_size.height_())
	{
		// Reduce size to the max lines
		m_text_backclip.height_(m_text_size.height_());
	}

	Margin marg;

	// Place the text and compute the size of text
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, text_align());

	// Adapt the fore clipping to the size of edit field
	m_foreclip = m_text_foreclip;
	m_foreclip.increase(padding());

	// Adapt the back clipping to the size of edit field
	m_backclip = m_foreclip;
	m_backclip.increase(margin());
}

void Edit::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Clipping background
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);

		String display;
		uint32_t txt_col = stated_color(m_text_color);
		
		if (m_input->size() == 0 && m_place_holder != 0)
		{
			display = *m_place_holder;
			txt_col = stated_color(m_place_holder_color);
		}
		else
		{
			display = *m_input;
		}

		// If mask selected
		if (m_mask)
		{
			MaskValidator validator(*m_mask);
			validator.display(m_text, display);
		}

		// If it is a password
		if (m_password && m_text.size() != 0)
		{
			// Hide all characters
			for (uint32_t i = 0; i < display.count(); i++)
			{
				if (display.get(i) != '\n')
				{
					display.replace('*',i);
				}
			}
		}

		if (m_font.get())
		{
			m_text_box.parse(m_text_foreclip, *m_font, display, m_cursor_position, m_selection_start, m_selection_end, (Align)m_text_align);
		}
		
		Rect::build_focused_polygon(m_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT, 
			stated_color(m_focus_color),
			m_focused);

		Widget::paint(region);

		// Get clipping text area
		Area text_clip(m_text_backclip);

		// Add cursor width and height
		Margin cursor_margin(1,1,1,1);
		text_clip.increase(cursor_margin);

		// Clipping text area
		region.intersect(text_clip);
		UIManager::renderer()->region(region);

		// Shift to show cursor
		m_text_box.text_shift(m_cursor_shift, m_text_foreclip);

		// Move to text to display cursor location
		Point text_shift(m_text_foreclip.position());
		text_shift.move(m_cursor_shift);

		m_text_box.paint(m_cursor_shift, *m_font.get(), display, m_text_foreclip.position(), m_text_backclip, txt_col,
			focused() ? stated_color(m_cursor_color) : Color::TRANSPARENT, 
			focused() ? stated_color(m_select_color) : Color::TRANSPARENT,
			(TypingMode)m_typing_mode == TypingMode::INSERTION);
	}
}


/** Get the widget hovered */
Widget * Edit::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Call back on key */
void Edit::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (!(evt.key() == (wchar_t)ReservedKey::KEY_ENTER && m_max_lines <= 1))
			{
				on_key_down(evt.key(), evt.modifier());
				m_text_modified = 1;
				UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
			}
		}
	}
}

/** Treat the click on widget, used to change the focus */
void Edit::on_select(Widget * widget, const SelectEvent & evt)
{
	if (m_text.size() > 0)
	{
		uint32_t to  =UINT32_MAX;

		select_font();

		if (evt.status() == SelectEvent::SELECT_START)
		{
			m_select_from = m_text_box.cursor_position(evt.position(), m_text_foreclip, m_cursor_shift, *m_font.get(), m_text, (Align)m_text_align);
		}
		else if (evt.status() == SelectEvent::SELECT_PROGRESS || evt.status() == SelectEvent::SELECT_END)
		{
			to   = m_text_box.cursor_position(evt.position(),   m_text_foreclip, m_cursor_shift, *m_font.get(), m_text, (Align)m_text_align);
		}
	
		if (to != UINT32_MAX && m_select_from != UINT32_MAX)
		{
			select_text(m_select_from,to);
		}

		if (evt.status() == SelectEvent::SELECT_END)
		{
			m_select_from = UINT32_MAX;
		}
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

/** Call back on click */
void Edit::on_click(Widget * widget, const ClickEvent & evt)
{
	if (m_text.size() > 0)
	{
		uint32_t position  =UINT32_MAX;

		select_font();
		position = m_text_box.cursor_position(evt.position(), m_text_foreclip, m_cursor_shift, *m_font.get(), m_text, (Align)m_text_align);
	
		if (position != UINT32_MAX)
		{
			select_text(position,position);
		}
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

/** Serialize the content of widget into json */
void Edit::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	EditStyle::serialize(it);

	if (m_mask)
	{
		it["mask"]      = *m_mask;
	}
}

/** Unserialize the content of widget from json */
void Edit::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	EditStyle::unserialize(it);

	String mask;
	mask   = (String)it["text"];
	if (mask.size())
	{
		if (m_mask)
		{
			delete m_mask;
		}
		m_mask = new String(mask);
	}
}

