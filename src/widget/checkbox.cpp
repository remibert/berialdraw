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
	bind(this, &Checkbox::on_key);
	bind(this, &Checkbox::on_click);
}

Checkbox::~Checkbox()
{
}

/** Copy all styles of the checkbox */
void Checkbox::copy(const Checkbox & checkbox)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&checkbox);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&checkbox);
	*((TextStyle*)this) = *(TextStyle*)(&checkbox);
	*((BorderStyle*)this) = *(BorderStyle*)(&checkbox);
	*((CheckboxStyle*)this) = *(CheckboxStyle*)(&checkbox);
}

/** Copy all styles of the checkbox */
void Checkbox::copy(const Checkbox * checkbox)
{
	if(checkbox)
	{
		copy(*checkbox);
	}
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
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		}
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	
	// Add text height in the icon content size
	if (m_text.size() > 0)
	{
		result.increase_(padding().left_() + m_text_size.width_() + padding().right_(), 0);
		if (m_text_size.height_() > m_checkbox_size.height_())
		{
			result.height_(m_text_size.height_());
		}
	}
	return result;
}

/** Place all widget in area */
void Checkbox::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place the check box
	m_check_foreclip = m_foreclip;

	// If text not empty
	if (m_text.size() > 0)
	{
		Margin marg;

		// Place switch text
		m_text_backclip = m_foreclip;
		marg.left_(m_checkbox_size.width_() + padding().left_());
		place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
		m_check_foreclip = m_text_foreclip;
		m_check_foreclip.size(m_checkbox_size);
		Coord move_y = (m_checkbox_size.height_() > m_text_size.height_() ? 0-((m_checkbox_size.height_() - m_text_size.height_())>>1) : ((m_text_size.height_()-m_checkbox_size.height_())>>1));
		m_check_foreclip.position().move_(0-(m_checkbox_size.width_() + padding().left_()), move_y);
		m_check_foreclip.position().nearest_pixel();
	}
}

void Checkbox::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		// Create an area for just the checkbox (not including text)
		Area area_box(m_check_foreclip);
		area_box.size().set_(m_checkbox_size.width_(), m_checkbox_size.height_());

		// Draw checkbox box
		Rect::build_focused_polygon(area_box, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color),
			stated_color(m_border_color),
			Color::TRANSPARENT,
			stated_color(m_focus_color),
			m_focused);

		// Draw check mark if checked
		if (m_checked)
		{
			Area area_check(area_box);
			area_check.size().decrease_(m_check_padding << 1, m_check_padding << 1);
			area_check.position().move_(m_check_padding, m_check_padding);

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

					Coord min_size = min(area_check.size().width_(), area_check.size().height_());
					polygon.zoom_((min_size << 6)/resolution);

					UIManager::renderer()->draw(polygon, Point(area_check.position().x_(), area_check.position().y_(),false));
				}
			}
			else
			{
				// Default check mark (simple square)
				Rect::build_polygon(area_check, m_radius, 0, 0, ALL_BORDERS, stated_color(m_check_color), 0);
			}
		}
		
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
Widget * Checkbox::hovered(const Region & parent_region, const Point & position)
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
void Checkbox::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	CheckboxStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Checkbox::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	CheckboxStyle::unserialize(it);
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


