#include "berialdraw_imp.hpp"

using namespace berialdraw;

TableView::TableView(Widget * parent):
	Widget("table_view", parent, sizeof(TableView)),
	m_scroll_view(nullptr),
	m_grid(nullptr)
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (TableViewStyle*)this);

	// Create internal scroll view
	m_scroll_view = new ScrollView(this);
	m_scroll_view->extend(Extend::EXTEND_ALL);
	m_scroll_view->align(Align::ALIGN_TOP_LEFT);
	m_scroll_view->scroll_direction(ScrollDirection::SCROLL_ALL_DIRECTIONS);

	// Create internal grid inside scroll view
	m_grid = new Grid(m_scroll_view);
	m_grid->extend(Extend::EXTEND_NONE);
	//m_grid->size_policy(SizePolicy::SHRINK_ALL);
}

TableView::~TableView()
{
}

void TableView::place(const Area & area, bool in_layout)
{
	// Set grid line thickness for cell placement
	if (m_grid)
	{
		m_grid->m_cells.horizontal_line_thickness(horizontal_thickness_());
		m_grid->m_cells.vertical_line_thickness(vertical_thickness_());
	}

	place_in_area(area, in_layout);

	// Place children in the foreclip area
	Widget* child = m_children;
	while (child)
	{
		child->place(m_foreclip, in_layout);
		child = child->next();
	}
}

void TableView::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If widget visible
	if (region.is_inside(m_foreclip.position(), m_foreclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		
		// Draw alternating row backgrounds
		if (m_grid)
		{
			const auto& row_positions = m_grid->m_cells.get_row_positions();
			Dim row_count = m_grid->m_cells.row_count();
			
			for (Dim row = 0; row < row_count; row++)
			{
				Dim row_y = row_positions[row];
				Dim row_height = m_grid->m_cells.row_height(row);
				
				// Create rectangle for this row in 64ths
				Area row_area(m_foreclip.x_(), row_y, m_foreclip.width_(), row_height, false);
				row_area.nearest_pixel();
				
				// Check if row is visible
				if (region.is_inside(row_area.position(), row_area.size()) != Region::OUT)
				{
					// Choose color based on alternating pattern
					uint32_t row_color = (row % 2 == 0) ? alternating_row_color1() : alternating_row_color2();
				
					// Create BorderStyle for grid appearance
					BorderStyle border_style;
					border_style.border_color(grid_color());
					border_style.focus_color(Color::TRANSPARENT);
				
					// Draw background with focus support
					Rect::build_focused_polygon(row_area,
						*(CommonStyle*)this,
						border_style,
						row_color,
						grid_color(),
						Color::TRANSPARENT,
						Color::TRANSPARENT,
						m_focused);
				}
			}
		}
		
		// Draw grid lines if visible
		if (m_grid && grid_visible())
		{
			Area foreclip = m_grid->foreclip();
			const auto& row_positions = m_grid->m_cells.get_row_positions();
			const auto& col_positions = m_grid->m_cells.get_col_positions();
			Dim row_count = m_grid->m_cells.row_count();
			Dim col_count = m_grid->m_cells.column_count();
			Dim horizontal_thickness = m_grid->m_cells.horizontal_line_thickness();
			Dim vertical_thickness = m_grid->m_cells.vertical_line_thickness();
			uint32_t line_color = grid_color();
			
			// Draw top border line
			if (row_count > 0)
			{
				Dim line_y = row_positions[0] - horizontal_thickness;
				
				Area line_area(foreclip.x_(), line_y, foreclip.width_(), horizontal_thickness, false);
				line_area.nearest_pixel();
				if (region.is_inside(line_area.position(), line_area.size()) != Region::OUT)
				{
					Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
				}
			}
			
			// Draw left border line
			if (col_count > 0)
			{
				Dim line_x = col_positions[0] - vertical_thickness;
				Area line_area(line_x, foreclip.y_(), vertical_thickness, foreclip.height_(), false);
				line_area.nearest_pixel();
				if (region.is_inside(line_area.position(), line_area.size()) != Region::OUT)
				{
					Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
				}
			}
			
			// Draw horizontal grid lines (between rows and bottom border)
			for (Dim row = 0; row < row_count; row++)
			{
				Dim row_y = row_positions[row];
				Dim row_height = m_grid->m_cells.row_height(row);
				Dim line_y;
				
				if (row == row_count - 1)
				{
					line_y = (row_y + row_height + 63) & 0xFFFFFFC0;
				}
				else
				{
					line_y = row_y + row_height;
				}

				Area line_area(foreclip.x_(), line_y, foreclip.width_(), horizontal_thickness, false);
				line_area.nearest_pixel();
				
				// Check if line is visible
				if (region.is_inside(line_area.position(), line_area.size()) != Region::OUT)
				{
					Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
				}
			}
			
			// Draw vertical grid lines (between columns and right border)
			for (Dim col = 0; col < col_count; col++)
			{
				Dim col_x = col_positions[col];
				Dim col_width = m_grid->m_cells.column_width(col);
				Dim line_x;

				if (col == col_count - 1)
				{
					line_x = (col_x + col_width + 63) & 0xFFFFFFC0;
				}
				else
				{
					line_x = col_x + col_width;
				}
				
				Area line_area(line_x, foreclip.y_(), vertical_thickness, foreclip.height_(), false);
				line_area.nearest_pixel();
				
				// Check if line is visible
				if (region.is_inside(line_area.position(), line_area.size()) != Region::OUT)
				{
					Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
				}
			}
		}
		
		// Paint children (scroll view and its content)
		Widget* child = m_children;
		while (child)
		{
			child->paint(region);
			child = child->next();
		}
	}
}

void TableView::set_widget(uint16_t row, uint16_t column, Widget* widget)
{
	if (widget && m_grid)
	{
		// Set the cell position
		widget->row(row);
		widget->column(column);

		// The widget must be a child of the grid
		if (widget->parent() != m_grid)
		{
			// If it has another parent, this might be an issue
			// In a real implementation, you might need to reparent it
		}
	}
}

Widget* TableView::get_widget(uint16_t row, uint16_t column)
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

void TableView::remove_widget(uint16_t row, uint16_t column)
{
	Widget* widget = get_widget(row, column);
	if (widget)
	{
		delete widget;
	}
}

void TableView::clear()
{
	if (m_grid)
	{
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

	if (region.is_inside(position) != Region::Overlap::OUT)
	{
		if (m_scroll_view)
		{
			Widget* hovered = m_scroll_view->hovered(region, position);
			if (hovered)
			{
				result = hovered;
			}
			else
			{
				result = this;
			}
		}
		else
		{
			result = this;
		}
	}

	return result;
}

/** Serialize the content of widget into json */
void TableView::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TableViewStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void TableView::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TableViewStyle::unserialize(it);
}

#ifdef _DEBUG
void TableView::test1()
{
	Window window;
	window.position(10, 10);
	window.size(400, 300);
	window.color(Color::WHITE_BLUE);

	TableView* table = new TableView(&window);
	table->size(380, 280);
	table->position(10, 10);
	
	
	// Configure grid styling
	table->grid_color(0xFF808080);  // Gray grid lines
	table->horizontal_thickness(1); // 2 pixels
	table->vertical_thickness(1);   // 2 pixels
	table->alternating_row_color1(0xFFE8F8FF);  // Pastel blue
	table->alternating_row_color2(0xFFF0F8E8);  // Pastel green

	for (uint16_t row = 0; row < 15; row++)
	{
		for (uint16_t column = 0; column < 10; column++)
		{
			Label* label = new Label(table);
			label->text("(%c:%d)",0x41 + row,column+1);
			label->cell(row,column);
			UIManager::desktop()->dispatch();
		}	
	}
	table->m_scroll_view->scroll_position(0,0);

//	UIManager::desktop()->dispatch("$(ui.tests)/out/table_view_1.svg");
while(1)
UIManager::desktop()->dispatch();
}

void TableView::test2()
{
	// More complex test with multiple widgets
}

void TableView::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test1();
		test2();
	}
}
#endif
