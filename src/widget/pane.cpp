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
/** Return the size of content without marges */
Size Pane::content_size()
{
	return Widget::children_size();
}

void Pane::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		// Consider the placement in layout
		in_layout = true;
	}

	// Place the rectangle in area
	place_in_area(area, in_layout);

	// Save backclip
	Area backclip(m_backclip);

	// Place all child in area
	Widget::place(m_foreclip,true);

	// Restore backclip
	m_backclip = m_foreclip;

	// Add border for the backclip
	m_backclip.position().move_(0-(m_thickness),0-(m_thickness));
	m_backclip.size().increase_(m_thickness+m_thickness,m_thickness+m_thickness);
}

void Pane::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Rect::build_polygon(m_foreclip, m_radius, m_thickness, 0, m_borders, stated_color(m_color), stated_color(m_border_color));
		Widget::paint(region);
	}
}

/** Serialize the content of widget into json */
void Pane::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Pane::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
}


