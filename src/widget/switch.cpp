#include "berialdraw_imp.hpp"

using namespace berialdraw;

Switch::Switch(Widget * parent):
	Widget("switch", parent, sizeof(Switch))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (SwitchStyle*)this);
	bind(this, &Switch::on_key);
	bind(this, &Switch::on_click);
}

Switch::~Switch()
{
}

/** Copy all styles of the switchar */
void Switch::copy(const Switch & switch_)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&switch_);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&switch_);
	*((TextStyle*)this) = *(TextStyle*)(&switch_);
	*((BorderStyle*)this) = *(BorderStyle*)(&switch_);
	*((SwitchStyle*)this) = *(SwitchStyle*)(&switch_);
}

/** Copy all styles of the switchar */
void Switch::copy(const Switch * switch_)
{
	if(switch_)
	{
		copy(*switch_);
	}
}

/** Return the size of content without margins */
Size Switch::content_size()
{
	Size result = m_switch_size;

	if(m_text_modified || m_font_modified)
	{
		Area area;
		select_font();
		if (m_font.get())
		{
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		}
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	
	// Add text size to the right if not empty
	if (m_text.size() > 0)
	{
		result.increase_(padding().left_() + (10<<6) + m_text_size.width_() + padding().right_(), 0);
		if (m_text_size.height_() > m_switch_size.height_())
		{
			result.height_(m_text_size.height_());
		}
	}
	return result;
}

/** Place all widget in area */
void Switch::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place the switch
	m_switch_foreclip = m_foreclip;

	// If text not empty
	if (m_text.size() > 0)
	{
		Margin marg;

		// Place switch text
		m_text_backclip = m_foreclip;
		marg.left_(m_switch_size.width_() + padding().left_());
		place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
		m_switch_foreclip = m_text_foreclip;
		m_switch_foreclip.size(m_switch_size);
		Coord move_y = (m_switch_size.height_() > m_text_size.height_() ? 0-((m_switch_size.height_() - m_text_size.height_())>>1) : ((m_text_size.height_()-m_switch_size.height_())>>1));
		m_switch_foreclip.position().move_(0-(m_switch_size.width_() + padding().left_()), move_y);
		m_switch_foreclip.position().nearest_pixel();
	}
}

/** Paint on screen memory the content of this widget */
void Switch::paint_switch(Region & region)
{
	uint32_t track_color = checked() ? stated_color(m_on_track_color) : stated_color(m_off_track_color);
	Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

	UIManager::renderer()->region(region);

	// Create an area for just the switch (not including text)
	Area area_track(m_switch_foreclip);

	Rect::build_focused_polygon(area_track, 
		*(CommonStyle*)this,
		*(BorderStyle*)this,
		stated_color(track_color), 
		stated_color(m_border_color),
		Color::TRANSPARENT,
		stated_color(m_focus_color),
		m_focused);

	Area area_thumb(area_track);

	area_thumb.size().decrease_(m_thumb_padding << 1, m_thumb_padding << 1);
	area_thumb.position().move_(m_thumb_padding, m_thumb_padding);

	// Change position of thumb
	if (m_checked)
	{
		if (m_extend & Extend::EXTEND_WIDTH)
		{
			area_thumb.position().move(m_foreclip.width() - m_switch_size.height(), 0);
		}
		else
		{
			area_thumb.position().move(m_switch_size.width() - m_switch_size.height(), 0);
		}
	}
	area_thumb.size().width(area_thumb.size().height());
	Rect::build_polygon(area_thumb, substract(m_radius, m_thumb_padding), 0, 0, ALL_BORDERS, stated_color(m_thumb_color), 0);

}


void Switch::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		paint_switch(region);
		
		// Paint text to the right
		if(m_text.size() > 0)
		{
			region.intersect(m_text_backclip);
			select_font();
			UIManager::renderer()->region(region);
			Point shift;
			m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, stated_color(m_text_color), 0, 0, true);
		}
	}
}

/** Get the widget hovered */
Widget * Switch::hovered(const Region & parent_region, const Point & position)
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

/** Serialize the content of widget into json */
void Switch::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	SwitchStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Switch::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	SwitchStyle::unserialize(it);
}


/** Call back on key */
void Switch::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_BACKSPACE || evt.key() == (wchar_t)ReservedKey::KEY_DELETE)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = (m_checked == 0 ? 1 : 0);
				UIManager::notifier()->check(m_checked, this);
			}
			else if (evt.key() == (wchar_t)ReservedKey::KEY_RIGHT)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = 1;
				UIManager::notifier()->check(m_checked, this);
			}
			else if (evt.key() == (wchar_t)ReservedKey::KEY_LEFT)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = 0;
				UIManager::notifier()->check(m_checked, this);
			}
			else if (evt.key() == (wchar_t)ReservedKey::KEY_SPACE)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = (m_checked == 0 ? 1 : 0);
				UIManager::notifier()->check(m_checked, this);
			}
		}
	}
}

/** Call back on click */
void Switch::on_click(Widget * widget, const ClickEvent & evt)
{
	m_checked = (m_checked == 0 ? 1 : 0);
	UIManager::notifier()->check(m_checked, this);
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
}


