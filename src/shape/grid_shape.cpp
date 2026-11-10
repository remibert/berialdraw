#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create a grid shape and add it to the canvas
GridShape::GridShape(Canvas * canvas):
	Shape(canvas, sizeof(GridShape)),
	m_polygon(nullptr)
{
	UIManager::styles()->apply("grid_shape", &m_grid_style);
}

// Set the number of rows
void GridShape::rows(Dim count)
{
	if (m_rows != count)
	{
		m_rows = count;
		// Invalidate the registered shape so its canvas redraws.
		UIManager::invalidator()->dirty(static_cast<void*>(static_cast<Shape*>(this)), Invalidator::REDRAW);
	}
}

// Set the number of columns
void GridShape::columns(Dim count)
{
	if (m_columns != count)
	{
		m_columns = count;
		// Invalidate the registered shape so its canvas redraws.
		UIManager::invalidator()->dirty(static_cast<void*>(static_cast<Shape*>(this)), Invalidator::REDRAW);
	}
}

// Paint the shape
void GridShape::paint(const Point & shift)
{
	m_polygon.clear();
	bool has_lines = false;

	if (m_grid_style.grid_visible() && m_size.is_defined())
	{
		Dim width = m_size.width_q6();
		Dim height = m_size.height_q6();
		Area grid_area(0, 0, width, height, false);
		Outline& outline = m_polygon.outline();

		struct GridLineData
		{
			Dim extent;
			Dim thickness;
			Dim divisions;
		};

		auto get_line_position = [](const void* context, Dim index) -> Coord
		{
			const GridLineData& data = *(const GridLineData*)context;
			Dim actual_thickness = data.thickness < data.extent ? data.thickness : data.extent;
			Dim start = 0;
			if (index == data.divisions)
			{
				start = data.extent - actual_thickness;
			}
			else if (index > 0)
			{
				uint64_t boundary = ((uint64_t)data.extent * index) / data.divisions;
				start = boundary > (actual_thickness >> 1) ? (Dim)(boundary - (actual_thickness >> 1)) : 0;
				if (start + actual_thickness > data.extent)
				{
					start = data.extent - actual_thickness;
				}
			}
			return (Coord)start;
		};

		// Provide uniform row boundaries to the shared grid painter.
		if (m_rows > 0 && height > 0 && m_grid_style.horizontal_thickness_q6() > 0)
		{
			Dim thickness = m_grid_style.horizontal_thickness_q6();
			GridLineData data = {height, thickness, m_rows};
			has_lines = GridPainter::add_horizontal_lines(outline, grid_area, 0, width,
				thickness, m_rows + 1, &data, get_line_position) || has_lines;
		}

		// Provide uniform column boundaries to the shared grid painter.
		if (m_columns > 0 && width > 0 && m_grid_style.vertical_thickness_q6() > 0)
		{
			Dim thickness = m_grid_style.vertical_thickness_q6();
			GridLineData data = {width, thickness, m_columns};
			has_lines = GridPainter::add_vertical_lines(outline, grid_area, 0, height,
				thickness, m_columns + 1, &data, get_line_position) || has_lines;
		}
	}

	if (has_lines)
	{
		Polygon copy(m_polygon);
		UIManager::renderer()->draw_outline(position(), margin(), shift, center(), m_grid_style.grid_color(), angle_q6(), copy.outline());
	}
}