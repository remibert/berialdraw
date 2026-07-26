#include "berialdraw_imp.hpp"

using namespace berialdraw;

Label::Label(Widget * parent):
	Widget("label", parent, sizeof(Label))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
}

Label::~Label()
{
}

/** Copy all styles of the label */
void Label::copy(const Label & obj)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&obj);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&obj);
	*((TextStyle*)this)   = *(TextStyle*)(&obj);
	*((PaddingStyle*)this) = *(PaddingStyle*)(&obj);
}

/** Copy all styles of the label */
void Label::copy(const Label * label)
{
	if(label)
	{
		copy(*label);
	}
}

/** Serialize the content of widget into json */
void Label::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	PaddingStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Label::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Label::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

Size Label::content_size()
{
	Size result;
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

	result = m_text_size;

	// Add padding and thickness in size
	result.increase(padding());
	return result;
}

void Label::place(const Area & area, bool in_layout)
{
	Margin marg;
	if (m_text_align != Align::ALIGN_DEFAULT && m_align == Align::ALIGN_DEFAULT)
	{
		m_align = m_text_align;
		m_text_align = Align::ALIGN_DEFAULT;
	}
	compute_widget_placement(area, in_layout, 0);
	place_in_layout(m_contentclip, m_text_size, marg, EXTEND_NONE, m_foreclip, m_text_align);
}

void Label::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);
	uint32_t color = parent_focus_color(stated_color(m_text_color));

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		select_font();
		UIManager::renderer()->region(region);

		m_text_box.paint(*m_font.get(), m_text, m_foreclip.position(), m_backclip, color);
	}
}

/** Get the widget hovered */
Widget * Label::hovered(const Region & parent_region, const Point & position)
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
