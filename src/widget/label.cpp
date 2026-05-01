#include "berialdraw_imp.hpp"

using namespace berialdraw;

Label::Label(Widget * parent):
	Widget("label", parent, sizeof(Label))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
}

Label::~Label()
{
}

/** Copy all styles of the label */
void Label::copy(const Label & label)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&label);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&label);
	*((TextStyle*)this)   = *(TextStyle*)(&label);
}

/** Copy all styles of the label */
void Label::copy(const Label * label)
{
	if(label)
	{
		copy(*label);
	}
}

Size Label::content_size()
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

void Label::place(const Area & area, bool in_layout)
{
	if (m_text_align != ALIGN_DEFAULT && m_align == ALIGN_DEFAULT)
	{
		m_align     = m_text_align;
		m_text_align = ALIGN_DEFAULT;
	}
	Margin marg;

	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	Area text_backclip = m_foreclip;
	text_backclip.decrease(padding());
	place_in_layout(text_backclip, m_text_size, marg, EXTEND_NONE, m_foreclip, (Align)m_text_align);
}

void Label::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		select_font();
		UIManager::renderer()->region(region);

		Point shift;
		m_text_box.paint(shift, *m_font.get(), m_text, m_foreclip.position(), m_backclip, stated_color(m_text_color), 0, 0, true);
	}
}

/** Get the widget hovered */
Widget * Label::hovered(const Region & parent_region, const Point & position)
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
void Label::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Label::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
}


