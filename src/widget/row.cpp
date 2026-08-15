#include "berialdraw_imp.hpp"

using namespace berialdraw;

Row::Row(Widget * parent):
	Widget("row", parent, sizeof(Row))
{
	m_color = Color::TRANSPARENT;
}

Row::~Row()
{
}

/** Serialize the content of widget into json */
void Row::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	Widget::serialize(it);
}

/** Unserialize the content of widget from json */
void Row::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Row::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Rework all widget to follow this layout flow constraint */
void Row::flow_place(const Area & area)
{
	Dim column = 0;
	Dim row = 0;
	Dim linear_size = 0;
	Widget* child = m_children;

	// Scan all widget children
	while (child)
	{
		Size size = child->marged_size();

		linear_size += size.width_q6();
		if (linear_size > area.width_q6())
		{
			row = 0;
			column ++;
			linear_size = size.width_q6();
		}
		child->cell(column,row++);
		child = child->next();
	}
}

/** Rework all widget to follow this layout constraint */
void Row::linear_place()
{
	Dim row = 0;
	Widget* child = m_children;

	// Scan all widget children
	while (child)
	{
		child->cell(0,row++);
		child = child->next();
	}
}

void Row::place(const Area & area, bool in_layout)
{
	if (UIManager::invalidator()->is_dirty(this))
	{
		compute_widget_placement(area, in_layout, 0, true);

		if (m_flow)
		{
			flow_place(m_foreclip);
		}
		else
		{
			linear_place();
			UIManager::invalidator()->undirty(this, Invalidator::GEOMETRY);
		}

		// Place cells within own bounds
		m_cells.place(this, m_foreclip);
	}
}

Size Row::content_size()
{
	if (m_flow == false)
	{
		linear_place();
	}
		
	m_cells.rebound(this);
	Size result = m_cells.calc_sizes(this);
	return result;
}

/** Get the widget hovered */
Widget * Row::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;
	Widget* child = m_children;

	// Scan all widget children
	while (child && result == 0)
	{
		result = child->hovered(parent_region, position);
		child = child->next();
	}
	return result;
}

/** Get the flow */
bool Row::flow() const
{
	return m_flow;
}

/** Set the flow */
void Row::flow(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	Window * window = dynamic_cast<Window*>(root());
	if (window)
	{
		window->force_flow_replacement();
	}
	m_flow = v;
}


