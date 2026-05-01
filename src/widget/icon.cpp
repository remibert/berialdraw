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
}

Icon::~Icon()
{
	delete m_sketch;
}

/** Copy all styles of the icon */
void Icon::copy(const Icon & icon)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&icon);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&icon);
	*((BorderStyle*)this) = *(BorderStyle*)(&icon);
	*((TextStyle*)this)   = *(TextStyle*)(&icon);
	*((IconStyle*)this)   = *(IconStyle*)(&icon);
}

/** Copy all styles of the icon */
void Icon::copy(const Icon * icon)
{
	if(icon)
	{
		copy(*icon);
	}
}

Dim Icon::compute_zoom(Dim value, Dim zoom)
{
	return (Dim)((((uint64_t)value * (uint64_t)zoom)) >> 6);
}


/** Return the icon size */
Size Icon::icon_size()
{
	if (m_icon_modified)
	{
		m_sketch->filename(m_filename);
		m_icon_modified = false;
		m_sketch->zoom_(m_zoom);
		if (m_sketch->load())
		{
			m_icon_size.set_(compute_zoom(m_sketch->resolution().width_(), zoom_()), compute_zoom(m_sketch->resolution().height_(), zoom_()));
			m_icon_size.height_(m_icon_size.height_() + icon_padding().bottom_() + icon_padding().top_());
			m_icon_size.width_ (m_icon_size.width_()  + icon_padding().left_()   + icon_padding().right_());

			if (m_children)
			{
				// Compute the children with marged size
				Size children_size = Widget::children_size();
				if (children_size.width_() > m_icon_size.width_())
				{
					m_icon_size.width_(children_size.width_());
				}
				if (children_size.height_() > m_icon_size.height_())
				{
					m_icon_size.height_(children_size.height_());
				}
			}
		}
		else
		{
			m_icon_size.set(0,0);
		}
	}
	return m_icon_size;
}

/** Return the size of content without marges */
Size Icon::content_size()
{
	Size result = icon_size();

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
	if (m_text_size.width_() > 0 && m_text_size.height_() > 0)
	{
		//result.height_(result.height_()+ padding().top_() + m_text_size.height_() + padding().bottom_());
		result.increase_(0, padding().top_() + m_text_size.height_() + padding().bottom_());
	}
	return result;
}

void Icon::place(const Area & area, bool in_layout)
{
	Margin marg;

	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place button text
	m_text_backclip = m_foreclip;
	marg.bottom_(padding().bottom_());
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
	// Place the icon
	m_icon_foreclip = m_foreclip;

	// Add text height in the icon content size
	if (m_text_size.width_() > 0 && m_text_size.height_() > 0)
	{
		m_icon_foreclip.size().decrease_(0,padding().top_() + m_text_size.height_() + padding().bottom_());
	}

	// Place all children
	Area backclip(m_backclip);
	Widget::place(m_foreclip,in_layout);

	// Restore backclip
	m_backclip = backclip;
}

void Icon::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Draw rectangle
	region.intersect(m_backclip);

	// If button visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Point shift;

		Rect::build_focused_polygon(m_icon_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT, 
			stated_color(m_focus_color),
			m_focused);

		// Paint children
		Widget::paint(region);

		// Paint icon
		m_sketch->size(m_icon_foreclip.size());
		m_sketch->paint(m_icon_foreclip, icon_padding(), stated_color(m_icon_color));

		// Paint text
		region.intersect(m_text_backclip);
		select_font();
		UIManager::renderer()->region(region);
		m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, stated_color(m_text_color), 0, 0, true);
	}
}

/** Get the widget hovered */
Widget * Icon::hovered(const Region & parent_region, const Point & position)
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
void Icon::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	IconStyle::serialize(it);
	TextStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Icon::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	IconStyle::unserialize(it);
	TextStyle::unserialize(it);
}


