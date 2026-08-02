#include "berialdraw_imp.hpp"

using namespace berialdraw;

Pane::Pane(Widget * parent):
	Widget("pane", parent, sizeof(Pane))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
}

Pane::~Pane()
{
}

/** Copy all styles of the pane */
void Pane::copy(const Pane & pane)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&pane);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&pane);
	*((BorderStyle*)this)   = *(BorderStyle*)(&pane);
}

/** Copy all styles of the pane */
void Pane::copy(const Pane * pane)
{
	if(pane)
	{
		copy(*pane);
	}
}

/** Serialize the content of widget into json */
void Pane::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Pane::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
}

StyleCascadeMode Pane::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Return the size of content without marges */
Size Pane::content_size()
{
	Size result = Widget::children_size();

	// Add padding and thickness in size
	result.increase(padding());
	result.increase_q6(m_thickness << 1, m_thickness << 1);

	return result;
}

void Pane::place(const Area & area, bool in_layout)
{
	compute_widget_placement(area, in_layout, m_thickness);

	// Place all children
	Widget::place_children(m_contentclip, true);
}

void Pane::paint(const Region & parent_region)
{
	Region region(parent_region); 

	// Draw rectangle
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		UIManager::renderer()->region(region);

		// Draw rectangle with border
		paint_background(m_foreclip, *(CommonStyle*)this, *(BorderStyle*)this);

		// Clip content
		region.intersect(m_contentclip);
		UIManager::renderer()->region(region);

		// Paint children
		Widget::paint(region);
	}
}
