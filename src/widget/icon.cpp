#include "berialdraw_imp.hpp"

using namespace berialdraw;

Icon::Icon(Widget * parent):
	Widget("icon", parent, sizeof(Icon))
{
	m_sketch = new Sketch(this);
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (IconStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
}

Icon::~Icon()
{
	delete m_sketch;
}

/** Copy all styles of the icon */
void Icon::copy(const Icon & obj)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&obj);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&obj);
	*((BorderStyle*)this) = *(BorderStyle*)(&obj);
	*((TextStyle*)this)   = *(TextStyle*)(&obj);
	*((IconStyle*)this)   = *(IconStyle*)(&obj);
	*((PaddingStyle*)this) = *(PaddingStyle*)(&obj);
}

/** Copy all styles of the icon */
void Icon::copy(const Icon * icon)
{
	if(icon)
	{
		copy(*icon);
	}
}

/** Serialize the content of widget into json */
void Icon::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	IconStyle::serialize(it);
	TextStyle::serialize(it);
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Icon::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	IconStyle::unserialize(it);
	TextStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Icon::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Return the icon size (area available for the sketch inside the frame) */
Size Icon::icon_size()
{
	if (m_icon_modified)
	{
		m_sketch->filename(m_filename);
		if (m_sketch->load())
		{
			// Icon size = frame size minus thickness (border) minus icon_padding
			m_icon_size = m_icon_frame_size;
			m_icon_size.decrease_q6(m_thickness << 1, m_thickness << 1);
			m_icon_size.decrease_q6(
				m_icon_padding.left_q6() + m_icon_padding.right_q6(),
				m_icon_padding.top_q6()  + m_icon_padding.bottom_q6());
		}
		else
		{
			m_icon_size.set(0, 0);
		}
		m_icon_modified = false;
	}
	return m_icon_size;
}

/** Return the size of content without margins */
Size Icon::content_size()
{
	// Compute icon size first (may update m_icon_size)
	icon_size();

	// Parse text if needed
	if (m_text.size() > 0)
	{
		if (m_text_modified || m_font_modified)
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
	}

	// Base content = icon_frame_size
	Size result = m_icon_frame_size;

	// If text exists, add text height + text_padding below
	if (m_text.size() > 0)
	{
		result.increase_q6(0, m_text_size.height_q6() + m_text_padding);
	}

	// Add widget padding and thickness
	result.increase(padding());
	result.increase_q6(m_thickness << 1, m_thickness << 1);

	return result;
}

void Icon::place(const Area & area, bool in_layout)
{
	Margin marg;

	// Compute standard widget placement (fills m_backclip, m_foreclip, m_contentclip)
	compute_widget_placement(area, in_layout, m_thickness);

	// Total content size (frame + text)
	Size total_content_size(m_icon_frame_size);
	if (m_text.size() > 0)
	{
		total_content_size.increase_q6(0, m_text_size.height_q6() + m_text_padding);
	}

	// Place the whole content block (frame + text) inside contentclip according to align()
	Area content_area;
	place_in_layout(m_contentclip, total_content_size, marg, Extend::EXTEND_NONE, content_area, align());

	// Frame is at the top of content_area, with icon_frame_size
	m_frame_foreclip.position(content_area.position());
	m_frame_foreclip.size(m_icon_frame_size);

	// Text is below the frame with text_padding gap
	if (m_text.size() > 0)
	{
		Coord text_y = content_area.y_q6() + (Coord)m_icon_frame_size.height_q6() + (Coord)m_text_padding;
		m_text_foreclip.position().set_q6(content_area.x_q6(), text_y);
		m_text_foreclip.size(m_text_size);

		// Center text horizontally relative to frame if text is narrower
		if (m_text_size.width_q6() < m_icon_frame_size.width_q6())
		{
			Coord text_x = content_area.x_q6() + (Coord)((m_icon_frame_size.width_q6() - m_text_size.width_q6()) >> 1);
			m_text_foreclip.x_q6(text_x);
		}
	}
}

void Icon::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Clip to widget bounds
	region.intersect(m_backclip);

	// Check visibility
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		UIManager::renderer()->region(region);

		// Draw rounded rectangle frame around icon
		paint_background(m_frame_foreclip, *(CommonStyle*)this, *(BorderStyle*)this);

		region.intersect(m_contentclip);

		// Draw icon centered inside frame (minus thickness and icon_padding)
		if (m_icon_size.width_q6() > 0 && m_icon_size.height_q6() > 0)
		{
			Area icon_area;
			Margin marg;
			place_in_layout(m_frame_foreclip, m_icon_size, marg, Extend::EXTEND_NONE, icon_area, Align::CENTER);

			m_sketch->size(m_icon_size);
			m_sketch->paint(icon_area, stated_color(m_icon_color));
		}

		// Draw text below the frame
		if (m_text.size() > 0)
		{
			Region text_region(region);
			text_region.intersect(m_text_foreclip);
			select_font();
			UIManager::renderer()->region(text_region);
			m_text_box.paint(*m_font.get(), m_text, m_text_foreclip.position(), m_text_foreclip, stated_color(m_text_color));
		}
	}
}

/** Get the widget hovered */
Widget * Icon::hovered(const Region & parent_region, const Point & position)
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
