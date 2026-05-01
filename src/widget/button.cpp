#include "berialdraw_imp.hpp"

using namespace berialdraw;

Button::Button(Widget * parent):
	Widget("button", parent, sizeof(Button))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
}

Button::~Button()
{
}

void Button::copy(const Button & button)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&button);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&button);
	*((BorderStyle*)this) = *(BorderStyle*)(&button);
	*((TextStyle  *)this) = *(TextStyle  *)(&button);
}

void Button::copy(const Button * button)
{
	if(button)
	{
		copy(*button);
	}
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
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		}
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}

	result = m_text_size;
	result.height_(result.height_()+ padding().bottom_() + padding().top_());
	result.width_(result.width_()  + padding().left_() + padding().right_());

	if (m_children)
	{
		// Compute the children with marged size
		Size children_size = Widget::children_size();
		if (children_size.width_() > result.width_())
		{
			result.width_(children_size.width_());
		}
		if (children_size.height_() > result.height_())
		{
			result.height_(children_size.height_());
		}
	}
	return result;
}

void Button::place(const Area & area, bool in_layout)
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
	m_text_backclip.decrease(padding());
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)m_text_align);

	// Place all children
	Area backclip(m_backclip);
	Widget::place(m_foreclip,in_layout);

	// Restore backclip
	m_backclip = backclip;
}

void Button::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Draw rectangle
	region.intersect(m_backclip);

	// If button visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Point shift;

		Rect::build_focused_polygon(m_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT, 
			stated_color(m_focus_color),
			m_focused);

		// Paint children
		Widget::paint(region);

		// Paint text
		region.intersect(m_text_backclip);
		select_font();
		UIManager::renderer()->region(region);
		m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, stated_color(m_text_color), 0, 0, true);
	}
}

/** Get the widget hovered */
Widget * Button::hovered(const Region & parent_region, const Point & position)
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
void Button::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Button::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
}

