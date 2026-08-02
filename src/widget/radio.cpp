#include "berialdraw_imp.hpp"

using namespace berialdraw;

Radio::Radio(Widget * parent):
	Widget("radio", parent, sizeof(Radio))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (RadioStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
	bind(this, &Radio::on_key);
	bind(this, &Radio::on_click);
}

Radio::~Radio()
{
}

/** Copy all styles of the radio button */
void Radio::copy(const Radio & obj)
{
	*((CommonStyle*)this)  = *(CommonStyle*)(&obj);
	*((WidgetStyle*)this)  = *(WidgetStyle*)(&obj);
	*((TextStyle*)this)    = *(TextStyle*)(&obj);
	*((BorderStyle*)this)  = *(BorderStyle*)(&obj);
	*((RadioStyle*)this)   = *(RadioStyle*)(&obj);
	*((PaddingStyle*)this) = *(PaddingStyle*)(&obj);
}

/** Copy all styles of the radio button */
void Radio::copy(const Radio * radio)
{
	if(radio)
	{
		copy(*radio);
	}
}

/** Serialize the content of widget into json */
void Radio::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	RadioStyle::serialize(it);
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Radio::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	RadioStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Radio::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Return the size of content without margins */
Size Radio::content_size()
{
	Size result = m_radio_size;

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
		if (m_text_size.height_q6() > m_radio_size.height_q6())
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
void Radio::place(const Area & area, bool in_layout)
{
	// Place the widget
	compute_widget_placement(area, in_layout, m_thickness);

	// Place the text and checkbox
	place_text_with_element(
		m_text_size,
		m_radio_size,
		m_text_padding,
		m_text_foreclip, // out
		m_radio_foreclip, // out
		(m_text_align | Align::ALIGN_BOTTOM),
		Extend::EXTEND_NONE);
}

void Radio::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		// Draw checkbox box
		paint_background(m_radio_foreclip, *(CommonStyle*)this, *(BorderStyle*)this);

		// Draw check mark if checked
		if (m_checked)
		{
			Area area_radio(m_radio_foreclip);
			area_radio.size().decrease_q6(m_radio_padding << 1, m_radio_padding << 1);
			area_radio.position().move_q6(m_radio_padding, m_radio_padding);

			// Parse and draw the radio sketch using VectorScript
			if (m_radio_sketch.size() > 0)
			{
				Polygon polygon(0);
				VectorsScript script(m_radio_sketch.c_str(), polygon);
				
				if (script.parse() == VectorsScript::SUCCESS)
				{
					Coord resolution = script.get('R');

					// Render the radio mark polygon in the specified color
					polygon.color(stated_color(m_radio_color));

					Coord min_size = min(area_radio.size().width_q6(), area_radio.size().height_q6());
					polygon.zoom_q6((min_size << 6)/resolution);

					UIManager::renderer()->draw(polygon, Point(area_radio.position().x_q6(), area_radio.position().y_q6(),false));
				}
			}
			else
			{
				// Default radio mark (simple square)
				Rect::paint_rounded_rect(area_radio, m_radius, 0, 0, ALL_BORDERS, stated_color(m_radio_color), 0);
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
Widget * Radio::hovered(const Region & parent_region, const Point & position)
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

/** Helper to deselect radios in the same group recursively */
void Radio::deselect_radio(Widget * widget, const String & my_group)
{
	if (widget)
	{
		// Check if this is a Radio widget
		Radio* radio = dynamic_cast<Radio*>(widget);
		if (radio && radio != this)
		{
			// Check if this radio is in the same group
			const String& other_group = radio->group();

			// If both groups are empty, they belong to the same (default) group
			// If this radio has a group, only deselect if other radio has the same group
			if ((my_group.size() == 0 && other_group.size() == 0) ||
				(my_group.size() > 0 && other_group == my_group))
			{
				if (radio->m_checked)
				{
					radio->m_checked = false;
					UIManager::invalidator()->dirty(radio, Invalidator::REDRAW);
				}
			}
		}

		// Recursively deselect in children
		Widget* child = widget->children();
		while (child)
		{
			deselect_radio(child, my_group);
			child = child->next();
		}
	}
}

/** Deselect all radio siblings with the same group in the window */
void Radio::deselect_all()
{
	// Get the root widget (window)
	Widget * window = root();
	
	if (window)
	{
		// Get the group name of this radio
		const String& my_group = group();

		// Recursively deselect all radios in the same group
		deselect_radio(window, my_group);
	}
}

/** Call back on key */
void Radio::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_SPACE)
			{
				// Select this radio
				m_checked = true;
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				
				// Deselect all radios in the same group in the window
				deselect_all();
			}
		}
	}
}

/** Call back on click */
void Radio::on_click(Widget * widget, const ClickEvent & evt)
{
	// Select this radio
	m_checked = true;
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	
	// Deselect all radios in the same group in the window
	deselect_all();
}
