#include "berialdraw_imp.hpp"

using namespace berialdraw;

Button::Button(Widget * parent):
	Widget("button", parent, sizeof(Button))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
}

Button::~Button()
{
}

void Button::copy(const Button & obj)
{
	*((CommonStyle*)this)  = *(CommonStyle *)(&obj);
	*((WidgetStyle*)this)  = *(WidgetStyle *)(&obj);
	*((BorderStyle*)this)  = *(BorderStyle *)(&obj);
	*((TextStyle  *)this)  = *(TextStyle   *)(&obj);
	*((PaddingStyle*)this) = *(PaddingStyle*)(&obj);
}

void Button::copy(const Button * button)
{
	if(button)
	{
		copy(*button);
	}
}

/** Serialize the content of widget into json */
void Button::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Button::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Button::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

Size Button::content_size()
{
	Size result;
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

	result = m_text_size;

	if (m_children)
	{
		// Compute the children with marged size
		Size children_size = Widget::children_size();
		if (children_size.width_q6() > result.width_q6())
		{
			result.width_q6(children_size.width_q6());
		}
		if (children_size.height_q6() > result.height_q6())
		{
			result.height_q6(children_size.height_q6());
		}
	}

	// Add padding and thickness in size
	result.increase(padding());
	result.increase_q6(m_thickness << 1, m_thickness << 1);
	return result;
}

void Button::place(const Area & area, bool in_layout)
{
	Margin marg;

	compute_widget_placement(area, in_layout, m_thickness);

	// Place button text
	place_in_layout(m_contentclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, m_text_align);

	// Place all children
	Widget::place_children(m_contentclip, in_layout);
}

void Button::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Draw rectangle
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		UIManager::renderer()->region(region);

		paint_background(m_foreclip, *(CommonStyle*)this, *(BorderStyle*)this);

		// Paint children
		Widget::paint(region);

		// Paint text
		region.intersect(m_contentclip);
		select_font();

		UIManager::renderer()->region(region);

		m_text_box.paint(*m_font.get(), m_text, m_text_foreclip.position(), m_contentclip, stated_color(m_text_color));
	}
}

/** Get the widget hovered */
Widget * Button::hovered(const Region & parent_region, const Point & position)
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
