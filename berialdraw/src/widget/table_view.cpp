#include "berialdraw_imp.hpp"

using namespace berialdraw;

TableView::TableView(Widget * parent):
	Widget("table_view", parent, sizeof(TableView)),
	m_scroll_view(nullptr),
	m_grid(nullptr)
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);

	// Create internal scroll view
	m_scroll_view = new ScrollView(this);
	m_scroll_view->extend(Extend::EXTEND_ALL);

	// Create internal grid inside scroll view
	m_grid = new Grid(m_scroll_view);
	m_grid->extend(Extend::EXTEND_ALL);
}

TableView::~TableView()
{
}

void TableView::set_widget(uint16_t row, uint16_t column, Widget* widget)
{
	if (!widget || !m_grid)
		return;

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

Widget* TableView::get_widget(uint16_t row, uint16_t column)
{
	if (!m_grid)
		return nullptr;

	// Search through grid's children
	Widget* child = m_grid->children();
	while (child)
	{
		if (child->row() == row && child->column() == column)
		{
			return child;
		}
		child = child->next();
	}

	return nullptr;
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
	if (m_grid)
		return m_grid->row_count();
	return 0;
}

Dim TableView::column_count() const
{
	if (m_grid)
		return m_grid->column_count();
	return 0;
}

void TableView::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		// Paint children (scroll view and its content)
		Widget* child = m_children;
		while (child)
		{
			child->paint(region);
			child = child->next();
		}
	}
}

void TableView::place(const Area & area, bool in_layout)
{
	place_in_area(area, in_layout);

	// Place children in the foreclip area
	Widget* child = m_children;
	while (child)
	{
		child->place(m_foreclip, in_layout);
		child = child->next();
	}
}

Widget * TableView::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	if (region.is_inside(position) != Region::Overlap::OUT)
	{
		if (m_scroll_view)
		{
			Widget* hovered = m_scroll_view->hovered(region, position);
			if (hovered)
				return hovered;
		}
		return this;
	}
	return nullptr;
}

/** Serialize the content of widget into json */
void TableView::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void TableView::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
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


	for (uint16_t row = 0; row < 1000; row++)
	{
		for (uint16_t column = 0; column < 8; column++)
		{
			Label* label = new Label(table);
			label->text("(%d,%d)",row,column);
			label->cell(row,column);
		}	
	}

//	UIManager::desktop()->dispatch("$(ui.tests)/out/table_view_1.svg");
UIManager::desktop()->mainloop();
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
