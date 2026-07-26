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
	UIManager::styles()->apply(this, (PaddingStyle*)this);
	bind(this, &Switch::on_key);
	bind(this, &Switch::on_click);
}

Switch::~Switch()
{
}

/** Copy all styles of the switchar */
void Switch::copy(const Switch & obj)
{
	*((CommonStyle*)this)  = *(CommonStyle*)(&obj);
	*((WidgetStyle*)this)  = *(WidgetStyle*)(&obj);
	*((TextStyle*)this)    = *(TextStyle*)(&obj);
	*((BorderStyle*)this)  = *(BorderStyle*)(&obj);
	*((SwitchStyle*)this)  = *(SwitchStyle*)(&obj);
	*((PaddingStyle*)this) = *(PaddingStyle*)(&obj);
}

/** Copy all styles of the switchar */
void Switch::copy(const Switch * switch_)
{
	if(switch_)
	{
		copy(*switch_);
	}
}

/** Serialize the content of widget into json */
void Switch::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	SwitchStyle::serialize(it);
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Switch::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	SwitchStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Switch::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
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
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, m_text_align);
		}
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	
	// If the text defined
	if (m_text.size() > 0)
	{
		// Enlarge the width with the text size
		result.increase_q6(m_text_size.width_q6() + m_text_padding, 0);

		// Enlarge the height if the text taller than check
		if (m_text_size.height_q6() > m_switch_size.height_q6())
		{
			result.height_q6(m_text_size.height_q6());
		}
	}

	// Add padding and thickness in size
	result.increase(padding());
	result.increase_q6(m_thickness << 1, m_thickness << 1);

	return result;
}

/** Place all widget in area */
void Switch::place(const Area & area, bool in_layout)
{
	// Place the widget
	compute_widget_placement(area, in_layout, m_thickness);

	// Place the text and checkbox
	place_text_with_element(
		m_text_size,
		m_switch_size,
		m_text_padding,
		m_text_foreclip, // out
		m_switch_foreclip, // out
		(m_text_align | Align::ALIGN_BOTTOM),
		(Extend)m_extend);
}

/** Paint on screen memory the content of this widget */
void Switch::paint_switch(Region & region)
{
	uint32_t track_color = checked() ? stated_color(m_on_track_color) : stated_color(m_off_track_color);
	Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

	UIManager::renderer()->region(region);

	// Create an area for just the switch (not including text)
	Area area_track(m_switch_foreclip);

	Rect::paint_focused_rounded_rect(area_track, 
		*(CommonStyle*)this,
		*(BorderStyle*)this,
		stated_color(track_color), 
		stated_color(m_border_color),
		Color::TRANSPARENT,
		stated_color(m_focus_color),
		m_focused);

	Area area_thumb(area_track);

	area_thumb.size().decrease_q6(m_thumb_padding << 1, m_thumb_padding << 1);
	area_thumb.position().move_q6(m_thumb_padding, m_thumb_padding);

	// Change position of thumb
	if (m_checked)
	{
		if (m_extend & Extend::EXTEND_WIDTH)
		{
			area_thumb.position().move(m_switch_foreclip.width() - m_switch_size.height(), 0);
		}
		else
		{
			area_thumb.position().move(m_switch_size.width() - m_switch_size.height(), 0);
		}
	}
	area_thumb.size().width(area_thumb.size().height());
	Rect::paint_rounded_rect(area_thumb, substract(m_radius, m_thumb_padding), 0, 0, ALL_BORDERS, stated_color(m_thumb_color), 0);
}


void Switch::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		paint_switch(region);
		
		// Paint text to the right
		if (m_text.size() > 0)
		{
			select_font();
			region.intersect(m_text_foreclip);
			UIManager::renderer()->region(region);
			m_text_box.paint(*m_font.get(), m_text, m_text_foreclip.position(), m_contentclip, stated_color(m_text_color));
		}
	}
}

/** Get the widget hovered */
Widget * Switch::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Overlap::OUT)
	{
		return this;
	}
	return 0;
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
