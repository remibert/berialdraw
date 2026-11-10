#include "berialdraw_imp.hpp"

using namespace berialdraw;

TableView::TableView(Widget * parent):
	ScrollableContent("table_view", parent, sizeof(TableView)),
	m_grid(nullptr),
	m_table_view_modified(1)
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (GridStyle*)this);
	UIManager::styles()->apply(this, (TableViewStyle*)this);

	// Create internal grid directly as a child of TableView
	m_grid = new Grid(this);
}

TableView::~TableView()
{
}

/** Serialize the content of widget into json */
void TableView::serialize(JsonIterator& it)
{
	it[StyleNames::WIDGET_TYPE] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	GridStyle::serialize(it);
	TableViewStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void TableView::unserialize(JsonIterator& it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	GridStyle::unserialize(it);
	TableViewStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode TableView::style_cascade_mode() const
{
	return StyleCascadeMode::CASCADING;
}

/** Return the size of content without marges */
Size TableView::content_size()
{
	// Set grid line thickness for cell placement
	if (m_grid)
	{
		m_grid->m_cells.horizontal_line_thickness(horizontal_thickness_q6());
		m_grid->m_cells.vertical_line_thickness(vertical_thickness_q6());
	}
	Size result = ScrollableContent::content_size();

	// Add padding and thickness in size
	result.increase(padding());
	result.increase_q6(m_thickness << 1, m_thickness << 1);
	return result;
}


void TableView::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	UIManager::renderer()->region(region);

	if (region.is_inside(m_foreclip.position(), m_foreclip.size()) != Overlap::OUT)
	{
		paint_row_backgrounds();
		paint_grid_lines();
	}

	ScrollableContent::paint(parent_region);
}

void TableView::paint_row_backgrounds()
{
	if (m_grid)
	{
		const auto& row_positions = m_grid->m_cells.get_row_positions();
		const auto& col_positions = m_grid->m_cells.get_col_positions();
		Dim row_count = m_grid->m_cells.row_count();
		Dim col_count = m_grid->m_cells.column_count();
		const Area& grid_foreclip = m_grid->foreclip();
		
		// Calculate grid content width
		Dim grid_width = grid_foreclip.width_q6();
		if (col_count > 0)
		{
			Dim last_col_x = col_positions[col_count - 1];
			Dim last_col_width = m_grid->m_cells.column_width(col_count - 1);
			grid_width = last_col_x + last_col_width - grid_foreclip.x_q6();
		}

		for (Dim row = 0; row < row_count; row++)
		{
			Dim row_y = row_positions[row];
			Dim row_height = m_grid->m_cells.row_height(row);
			
			// Create rectangle for this row in 64ths, limited to grid content width
			Area row_area(grid_foreclip.x_q6(), row_y, grid_width, row_height, false);
			row_area.nearest_pixel();
			row_area.clip(m_foreclip);
			
			if (row_area.is_not_empty())
			{
				// Choose color based on alternating pattern
				uint32_t row_color = (row % 2 == 0) ? alternating_row_color1() : alternating_row_color2();
				
				// Create BorderStyle for grid appearance
				BorderStyle border_style;
				border_style.focus_color(Color::TRANSPARENT);
				
				// Draw background with focus support
				RectRenderer::paint_rect(row_area,row_color);
			}
		}
	}
}

void TableView::paint_grid_lines()
{
	if (m_grid && grid_visible())
	{
		const Area& foreclip = m_grid->foreclip();
		const Area& visible_area = m_foreclip;
		const Dim* row_positions = m_grid->m_cells.get_row_positions();
		const Dim* col_positions = m_grid->m_cells.get_col_positions();
		Dim row_count = m_grid->m_cells.row_count();
		Dim col_count = m_grid->m_cells.column_count();
		Dim horizontal_thickness = m_grid->m_cells.horizontal_line_thickness();
		Dim vertical_thickness = m_grid->m_cells.vertical_line_thickness();
		uint32_t line_color = grid_color();
		Outline outline;
		bool has_lines = false;
		struct CellGridLineData
		{
			const Cells* cells;
			const Dim* positions;
			Dim cell_count;
			Dim thickness;
			bool horizontal;
		};

		auto get_line_position = [](const void* context, Dim index) -> Coord
		{
			const CellGridLineData& data = *(const CellGridLineData*)context;
			int64_t position;
			if (index == 0)
			{
				position = (int64_t)data.positions[0] - data.thickness;
			}
			else
			{
				Dim cell = index - 1;
				Dim extent = data.horizontal ? data.cells->row_height(cell) : data.cells->column_width(cell);
				position = (int64_t)data.positions[cell] + extent;
				if (cell == data.cell_count - 1)
				{
					position = (position + 63) & ~INT64_C(63);
				}
			}
			return (Coord)position;
		};

		if (row_positions && row_count > 0 && horizontal_thickness > 0)
		{
			CellGridLineData data = {&m_grid->m_cells, row_positions, row_count, horizontal_thickness, true};
			has_lines = GridPainter::add_horizontal_lines(outline, visible_area, foreclip.x_q6(), foreclip.width_q6(),
				horizontal_thickness, row_count + 1, &data, get_line_position) || has_lines;
		}

		if (col_positions && col_count > 0 && vertical_thickness > 0)
		{
			CellGridLineData data = {&m_grid->m_cells, col_positions, col_count, vertical_thickness, false};
			has_lines = GridPainter::add_vertical_lines(outline, visible_area, foreclip.y_q6(), foreclip.height_q6(),
				vertical_thickness, col_count + 1, &data, get_line_position) || has_lines;
		}

		if (has_lines)
		{
			Point origin(0, 0, false);
			Point center(0, 0, false);
			Margin margin;
			UIManager::renderer()->draw_outline(origin, margin, origin, center, line_color, 0, outline);
		}
	}
}

void TableView::set_widget(Dim row, Dim column, Widget* widget)
{
	if (widget && m_grid)
	{
		// Set the cell position
		widget->row(row);
		widget->column(column);
		m_table_view_modified = 1;

		// The widget must be a child of the grid
		if (widget->parent() != m_grid)
		{
			// If it has another parent, this might be an issue
			// In a real implementation, you might need to reparent it
		}
	}
}

Widget* TableView::get_widget(Dim row, Dim column)
{
	Widget* result = nullptr;

	if (m_grid)
	{
		// Search through grid's children
		Widget* child = m_grid->children();
		while (child && !result)
		{
			if (child->row() == row && child->column() == column)
			{
				result = child;
			}
			if (!result)
			{
				child = child->next();
			}
		}
	}

	return result;
}

void TableView::remove_widget(Dim row, Dim column)
{
	Widget* widget = get_widget(row, column);
	if (widget)
	{
		m_table_view_modified = 1;
		delete widget;
	}
}

void TableView::clear()
{
	if (m_grid)
	{
		m_table_view_modified = 1;
		m_grid->clear();
	}
}

Dim TableView::row_count() const
{
	Dim result = 0;

	if (m_grid)
	{
		result = m_grid->row_count();
	}

	return result;
}

Dim TableView::column_count() const
{
	Dim result = 0;

	if (m_grid)
	{
		result = m_grid->column_count();
	}

	return result;
}

Widget * TableView::hovered(const Region & parent_region, const Point & position)
{
	Widget* result = nullptr;

	Region region(parent_region);
	region.intersect(m_foreclip);

	if (region.is_inside(position) != Overlap::OUT)
	{
		result = this;
		if (m_grid)
		{
			Widget* hovered = m_grid->hovered(region, position);
			if (hovered)
			{
				result = hovered;
			}
		}
	}
	return result;
}

void TableView::load(TextStream& stream)
{
	clear();
	load_json_from_stream(stream);
}

void TableView::load_json_from_stream(TextStream& stream)
{
	Json json;
	try
	{
		json.unserialize(stream);
		
		if (json.count() > 0)
		{
			// Load as direct array format: [[...], [...], ...]
			for (int32_t row = 0; row < json.count(); row++)
			{
				JsonIterator row_it = json[row];
				int32_t col_count = row_it.count();
				
				for (int32_t col = 0; col < col_count; col++)
				{
					Label* label = new Label(m_grid);
					String text = row_it[col] | "";
					label->text(text.c_str());
					label->cell(row, col);
				}
			}
		}
	}
	catch(...)
	{
		bd_printf("Unable to decode the table view\n");
	}
}

void TableView::save(TextStream& stream)
{
	Json json;
	populate_json_from_data(json);
	json.serialize(stream);
}

void TableView::populate_json_from_data(Json& json)
{
	JsonIterator it(json);
	
	// Save as direct array format: [[...], [...], ...]
	for (Dim row = 0; row < row_count(); row++)
	{
		for (Dim col = 0; col < column_count(); col++)
		{
			Widget* widget = get_widget(row, col);
			String cell_value;
			
			if (widget)
			{
				Label* label = dynamic_cast<Label*>(widget);
				if (label)
				{
					cell_value = label->text();
				}
			}
			
			it[row][col] = cell_value.c_str();
		}
	}
}
