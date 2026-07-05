#include "berialdraw_imp.hpp"

using namespace berialdraw;

Grid::Grid(Widget * parent):
	Widget("grid", parent, sizeof(Grid))
{
	m_color = Color::TRANSPARENT;
}

Grid::~Grid()
{
}

void Grid::place(const Area & area, bool in_layout)
{
	if (UIManager::invalidator()->is_dirty(this))
	{
		// Only use absolute placement when explicitly positioned or sized
		bool is_placed = m_size.is_width_defined() || m_size.is_height_defined() || 
		                 m_position.is_x_defined() || m_position.is_y_defined();

		if (is_placed)
		{
			// Absolute mode: compute bounds from position/size
			place_in_area(area, false);
		}
		else
		{
			// Layout mode: fill the available area minus margin
			m_backclip = area;
			m_foreclip = area;
			m_foreclip.decrease(margin());
		}

		// Place cells within own bounds
		m_cells.place(this, m_foreclip);
		UIManager::invalidator()->undirty(this, Invalidator::GEOMETRY);
	}
}

Size Grid::content_size()
{
	m_cells.rebound(this);
	Size result = m_cells.calc_sizes(this);
	return result;
}

/** Get the widget hovered */
Widget * Grid::hovered(const Region & parent_region, const Point & position)
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

/** Serialize the content of widget into json */
void Grid::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	WidgetStyle::serialize(it);
	CommonStyle::serialize(it);
	Widget::serialize(it);
}

/** Unserialize the content of widget from json */
void Grid::unserialize(JsonIterator & it)
{
	WidgetStyle::unserialize(it);
	CommonStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Grid::style_cascade_mode() const
{
	return StyleCascadeMode::TRANSPARENT;
}

Dim Grid::row_count() const
{
	return m_cells.row_count();
}

Dim Grid::column_count() const
{
	return m_cells.column_count();
}

