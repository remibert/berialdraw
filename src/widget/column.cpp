#include "berialdraw_imp.hpp"

using namespace berialdraw;

Column::Column(Widget * parent):
	Widget("column", parent, sizeof(Column))
{
	m_color = Color::TRANSPARENT;
}

Column::~Column()
{
}

/** Serialize the content of widget into json */
void Column::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	Widget::serialize(it);
}

/** Unserialize the content of widget from json */
void Column::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Column::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

/** Rework all widget to follow this layout flow constraint */
void Column::flow_place(const Area & area)
{
	uint16_t column = 0;
	uint16_t row = 0;
	Dim linear_size = 0;
	Widget* child = m_children;

	// Scan all widget children
	while (child)
	{
		Size size = child->marged_size();

		linear_size += size.height_q6();
		if (linear_size > area.height_q6())
		{
			column = 0;
			row ++;
			linear_size = size.height_q6();
		}
		child->cell(column++,row);
		child = child->next();
	}
}

/** Rework all widget to follow this layout constraint */
void Column::linear_place()
{
	uint16_t column = 0;
	Widget* child = m_children;

	// Scan all widget children
	while (child)
	{
		child->cell(column++,0);
		child = child->next();
	}
}

void Column::place(const Area & area, bool in_layout)
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

Size Column::content_size()
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
Widget * Column::hovered(const Region & parent_region, const Point & position)
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
bool Column::flow() const
{
	return m_flow;
}

/** Set the flow */
void Column::flow(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	Window * window = dynamic_cast<Window*>(root());
	if (window)
	{
		window->force_flow_replacement();
	}
	m_flow = v;
}


