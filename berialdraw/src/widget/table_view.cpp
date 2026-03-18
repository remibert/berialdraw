#include "berialdraw_imp.hpp"

using namespace berialdraw;

TableView::TableView(Widget * parent):
	ScrollableContent("table_view", parent, sizeof(TableView)),
	m_grid(nullptr),
	m_content_size(),
	m_table_view_modified(1)
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (TableViewStyle*)this);

	// Create internal grid directly as a child of TableView
	m_grid = new Grid(this);
}

TableView::~TableView()
{
}

/** Return the size of content without marges */
Size TableView::content_size()
{
	// Set grid line thickness for cell placement
	if (m_grid)
	{
		m_grid->m_cells.horizontal_line_thickness(horizontal_thickness_());
		m_grid->m_cells.vertical_line_thickness(vertical_thickness_());
	}	
	return ScrollableContent::content_size();
}


void TableView::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	UIManager::renderer()->region(region);

	if (region.is_inside(m_foreclip.position(), m_foreclip.size()) != Region::OUT)
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
		Dim grid_width = grid_foreclip.width_();
		if (col_count > 0)
		{
			Dim last_col_x = col_positions[col_count - 1];
			Dim last_col_width = m_grid->m_cells.column_width(col_count - 1);
			grid_width = last_col_x + last_col_width - grid_foreclip.x_();
		}

		for (Dim row = 0; row < row_count; row++)
		{
			Dim row_y = row_positions[row];
			Dim row_height = m_grid->m_cells.row_height(row);
			
			// Create rectangle for this row in 64ths, limited to grid content width
			Area row_area(grid_foreclip.x_(), row_y, grid_width, row_height, false);
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
				Rect::build_focused_polygon(row_area,
					*(CommonStyle*)this,
					border_style,
					row_color,
					Color::TRANSPARENT,
					Color::TRANSPARENT,
					Color::TRANSPARENT,
					m_focused);
			}
		}
	}
}

void TableView::paint_grid_lines()
{
	if (m_grid && grid_visible())
	{
		paint_top_border_line();
		paint_left_border_line();
		paint_horizontal_lines();
		paint_vertical_lines();
	}
}

void TableView::paint_top_border_line()
{
	const Dim* row_positions = m_grid->m_cells.get_row_positions();
	if (row_positions)
	{
		const Area& foreclip = m_grid->foreclip();
		Dim horizontal_thickness = m_grid->m_cells.horizontal_line_thickness();
		uint32_t line_color = grid_color();
		
		Dim line_y = row_positions[0] - horizontal_thickness;
		
		Area line_area(foreclip.x_(), line_y, foreclip.width_(), horizontal_thickness, false);
		line_area.nearest_pixel();
		line_area.clip(m_foreclip);
		if (line_area.is_not_empty())
		{
			Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
		}
	}
}

void TableView::paint_left_border_line()
{
	const Dim* col_positions = m_grid->m_cells.get_col_positions();
	if (col_positions)
	{
		const Area& foreclip = m_grid->foreclip();
		Dim vertical_thickness = m_grid->m_cells.vertical_line_thickness();
		uint32_t line_color = grid_color();
		
		Dim line_x = col_positions[0] - vertical_thickness;
		Area line_area(line_x, foreclip.y_(), vertical_thickness, foreclip.height_(), false);
		line_area.nearest_pixel();
		line_area.clip(m_foreclip);
		if (line_area.is_not_empty())
		{
			Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
		}
	}
}

void TableView::paint_horizontal_lines()
{
	const Dim* row_positions = m_grid->m_cells.get_row_positions();
	Dim row_count = m_grid->m_cells.row_count();
	const Area& foreclip = m_grid->foreclip();
	Dim horizontal_thickness = m_grid->m_cells.horizontal_line_thickness();
	uint32_t line_color = grid_color();
	
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
		line_area.clip(m_foreclip);
		
		if (line_area.is_not_empty())
		{
			Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
		}
	}
}

void TableView::paint_vertical_lines()
{
	const Dim* col_positions = m_grid->m_cells.get_col_positions();
	Dim col_count = m_grid->m_cells.column_count();
	const Area& foreclip = m_grid->foreclip();
	Dim vertical_thickness = m_grid->m_cells.vertical_line_thickness();
	uint32_t line_color = grid_color();
	
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
		line_area.clip(m_foreclip);
		
		if (line_area.is_not_empty())
		{
			Rect::build_polygon(line_area, 0, 0, 0, ALL_BORDERS, line_color, Color::TRANSPARENT);
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
		m_table_view_modified = 1;

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
	Widget* result = this;

	Region region(parent_region);
	region.intersect(m_foreclip);

	if (region.is_inside(position) != Region::Overlap::OUT)
	{
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

void TableView::load(TextStream& stream)
{
	clear();
	load_json_from_stream(stream);
}

void TableView::load_json_from_stream(TextStream& stream)
{
	Json json;
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

#ifdef _DEBUG
/** Test 1: Basic grid with scrolling */
void TableView::test1()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	
	for (uint16_t row = 0; row < 30; row++)
	{
		for (uint16_t column = 0; column < 20; column++)
		{
			Label* label = new Label(table);
			label->text("(%c:%d)", 0x41 + row, column + 1);
			label->cell(row, column);
		}
	}
	
	String script1(
	"["
		"{'type':'touch','x':200,'y':200,'state':'down'},"
		"{'type':'touch','x':190,'y':190,'state':'move'},"
		"{'type':'touch','x':185,'y':185,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}"
	"]");
	UIManager::notifier()->play_script(script1, "$(ui.tests)/out/tableview1.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 2: Grid with JSON load */
void TableView::test2()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	ScrollView* scrollview = new ScrollView(&window);
	Column* column = new Column(scrollview);
	
	TableView* table = new TableView(column);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	table->grid_color(Color::DARK_GRAY);
	
	String json_data(
		"["
		"['ID','Name','Email'],"
		"['001','Alice','alice@example.com'],"
		"['002','Bob','bob@example.com'],"
		"['003','Charlie','charlie@example.com'],"
		"['004','Diana','diana@example.com'],"
		"['005','Eve','eve@example.com'],"
		"]"
	);
	
	json_data.offset(0);
	table->load(json_data);
	
	String script2("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script2, "$(ui.tests)/out/tableview2.svg");
	// UIManager::desktop()->mainloop();
}

/** Test 3: Grid visibility toggle */
void TableView::test3()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_visible(true);
	table->grid_color(Color::RED);
	table->horizontal_thickness(2);
	table->vertical_thickness(2);
	
	for (uint16_t row = 0; row < 5; row++)
	{
		for (uint16_t col = 0; col < 5; col++)
		{
			Label* label = new Label(table);
			label->text("%d", row * 5 + col + 1);
			label->cell(row, col);
		}
	}
	
	String script3("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script3, "$(ui.tests)/out/tableview3.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 4: Grid with invisible grid lines */
void TableView::test4()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_visible(false);
	table->alternating_row_color1(Color::ALICE_BLUE);
	table->alternating_row_color2(Color::LIGHT_AQUAMARINE);
	
	for (uint16_t row = 0; row < 8; row++)
	{
		for (uint16_t col = 0; col < 4; col++)
		{
			Label* label = new Label(table);
			label->text("Cell %d", row * 4 + col + 1);
			label->cell(row, col);
		}
	}
	
	String script4("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script4, "$(ui.tests)/out/tableview4.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 5: Different grid colors and thickness */
void TableView::test5()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::BLUE);
	table->horizontal_thickness(3);
	table->vertical_thickness(1);
	table->alternating_row_color1(Color::WHITE);
	table->alternating_row_color2(Color::DARK_GRAY);
	
	for (uint16_t row = 0; row < 6; row++)
	{
		for (uint16_t col = 0; col < 6; col++)
		{
			Label* label = new Label(table);
			label->text("R%dC%d", row, col);
			label->cell(row, col);
		}
	}
	
	String script5("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script5, "$(ui.tests)/out/tableview5.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 6: Progressive grid filling 1x1 to 10x10 with snapshots */
void TableView::test6()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	table->alternating_row_color1(Color::WHITE);
	table->alternating_row_color2(Color::WHITE_SMOKE);
	
	int id = 0;
	for (uint16_t size = 1; size <= 10; size++)
	{
		table->clear();
		
		for (uint16_t row = 0; row < size; row++)
		{
			for (uint16_t col = 0; col < size; col++)
			{
				Label* label = new Label(table);
				label->text("%d", row * size + col + 1);
				label->cell(row, col);
			}
		}

		String name;
		name.print("$(ui.tests)/out/tableview6_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
	//UIManager::desktop()->mainloop();
}

/** Test 7: Scrolling behavior test */
void TableView::test7()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	
	for (uint16_t row = 0; row < 20; row++)
	{
		for (uint16_t col = 0; col < 15; col++)
		{
			Label* label = new Label(table);
			label->text("R%dC%d", row, col);
			label->cell(row, col);
		}
	}
	
	table->scroll_position(0, 0);
	
	String script7(
	"["
		"{'type':'key','key':9208,'state':'press'}," // initial
		"{'type':'touch','x':200,'y':200,'state':'down'},"
		"{'type':'touch','x':250,'y':250,'state':'move'},"
		"{'type':'touch','x':250,'y':250,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}," // after vertical scroll
		"{'type':'touch','x':200,'y':200,'state':'down'},"
		"{'type':'touch','x':150,'y':200,'state':'move'},"
		"{'type':'touch','x':150,'y':200,'state':'up'},"
		"{'type':'key','key':9208,'state':'press'}" // after horizontal scroll
	"]");
	UIManager::notifier()->play_script(script7, "$(ui.tests)/out/tableview7.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 8: Custom colors and row styling */
void TableView::test8()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::GREEN);
	table->horizontal_thickness(2);
	table->vertical_thickness(2);
	table->alternating_row_color1(Color::LIGHT_AQUAMARINE);
	table->alternating_row_color2(Color::ALICE_BLUE);
	
	for (uint16_t row = 0; row < 10; row++)
	{
		for (uint16_t col = 0; col < 8; col++)
		{
			Label* label = new Label(table);
			label->text("%d", (row + 1) * (col + 1));
			label->cell(row, col);
		}
	}
	
	String script8("[{'type':'key','key':9208,'state':'press'}]");
	UIManager::notifier()->play_script(script8, "$(ui.tests)/out/tableview8.svg");
	//UIManager::desktop()->mainloop();
}

/** Test 9: Large table with scrolling snapshot */
void TableView::test9()
{
	Window window;
	window.color(Color::WHITE_BLUE);
	
	TableView* table = new TableView(&window);
	table->grid_color(Color::DARK_GRAY);
	table->horizontal_thickness(1);
	table->vertical_thickness(1);
	
	for (uint16_t row = 0; row < 50; row++)
	{
		for (uint16_t col = 0; col < 25; col++)
		{
			Label* label = new Label(table);
			label->text("Val");
			label->cell(row, col);
		}
	}
	
	String script9("[{'type':'key','key':9208,'state':'press'}]"); // Large table top-left
	UIManager::notifier()->play_script(script9, "$(ui.tests)/out/tableview9.svg");
	//UIManager::desktop()->mainloop();
}

/** Main test runner - execute all tests */
void TableView::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test9();
		test8();
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
