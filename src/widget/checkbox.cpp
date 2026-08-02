#include "berialdraw_imp.hpp"

using namespace berialdraw;

Checkbox::Checkbox(Widget * parent):
	Widget("checkbox", parent, sizeof(Checkbox))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (CheckboxStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
	bind(this, &Checkbox::on_key);
	bind(this, &Checkbox::on_click);
}

Checkbox::~Checkbox()
{
}

/** Copy all styles of the checkbox */
void Checkbox::copy(const Checkbox & obj)
{
	*((CommonStyle*)this)  = *(CommonStyle*)(&obj);
	*((WidgetStyle*)this)  = *(WidgetStyle*)(&obj);
	*((TextStyle*)this)    = *(TextStyle*)(&obj);
	*((BorderStyle*)this)  = *(BorderStyle*)(&obj);
	*((CheckboxStyle*)this) = *(CheckboxStyle*)(&obj);
	*((PaddingStyle*)this) = *(PaddingStyle*)(&obj);
}

/** Copy all styles of the checkbox */
void Checkbox::copy(const Checkbox * checkbox)
{
	if(checkbox)
	{
		copy(*checkbox);
	}
}

/** Serialize the content of widget into json */
void Checkbox::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	CheckboxStyle::serialize(it);
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Checkbox::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	CheckboxStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Checkbox::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Return the size of content without margins */
Size Checkbox::content_size()
{
	Size result = m_checkbox_size;

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
		if (m_text_size.height_q6() > m_checkbox_size.height_q6())
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
void Checkbox::place(const Area& area, bool in_layout)
{
	// Place the widget
	compute_widget_placement(area, in_layout, m_thickness);

	// Place the text and checkbox
	place_text_with_element(
		m_text_size,
		m_checkbox_size,
		m_text_padding,
		m_text_foreclip, // out
		m_check_foreclip, // out
		(m_text_align | Align::ALIGN_BOTTOM),
		Extend::EXTEND_NONE);
}

void Checkbox::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		// Draw checkbox box
		paint_background(m_check_foreclip, *(CommonStyle*)this, *(BorderStyle*)this);

		// Draw check mark if checked
		if (m_checked)
		{
			Area area_check(m_check_foreclip);
			area_check.size().decrease_q6(m_check_padding << 1, m_check_padding << 1);
			area_check.position().move_q6(m_check_padding, m_check_padding);

			// Parse and draw the check sketch using VectorScript
			if (m_check_sketch.size() > 0)
			{
				Polygon polygon(0);
				VectorsScript script(m_check_sketch.c_str(), polygon);
				
				if (script.parse() == VectorsScript::SUCCESS)
				{
					Coord resolution = script.get('R');

					// Render the check mark polygon in the specified color
					polygon.color(stated_color(m_check_color));

					Coord min_size = min(area_check.size().width_q6(), area_check.size().height_q6());
					polygon.zoom_q6((min_size << 6)/resolution);

					UIManager::renderer()->draw(polygon, Point(area_check.position().x_q6(), area_check.position().y_q6(),false));
				}
			}
			else
			{
				// Default check mark (simple square)
				Rect::paint_rounded_rect(area_check, m_radius, 0, 0, ALL_BORDERS, stated_color(m_check_color), 0);
			}
		}
		
		// Paint text to the right
		if(m_text.size() > 0)
		{
			select_font();
			region.intersect(m_text_foreclip);
			UIManager::renderer()->region(region);
			m_text_box.paint(*m_font.get(), m_text, m_text_foreclip.position(), m_contentclip, stated_color(m_text_color));
		}
	}
}

/** Get the widget hovered */
Widget * Checkbox::hovered(const Region & parent_region, const Point & position)
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
void Checkbox::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_BACKSPACE || evt.key() == (wchar_t)ReservedKey::KEY_DELETE || evt.key() == (wchar_t)ReservedKey::KEY_SPACE)
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
void Checkbox::on_click(Widget * widget, const ClickEvent & evt)
{
	m_checked = (m_checked == 0 ? 1 : 0);
	UIManager::notifier()->check(m_checked, this);
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
}
