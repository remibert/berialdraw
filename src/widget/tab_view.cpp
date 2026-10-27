#include "berialdraw_imp.hpp"

using namespace berialdraw;

enum TabId
{
	FIRST_TAB  = 0x20000000,
	FIRST_PAGE = 0x40000000,
};



// Create a tab view.
TabView::TabView(Widget * parent):
	Widget("tab_view", parent, sizeof(TabView))
{
	UIManager::styles()->apply(this, (CommonStyle *)this);
	UIManager::styles()->apply(this, (WidgetStyle *)this);
	UIManager::styles()->apply(this, (PaddingStyle *)this);
	UIManager::styles()->apply(this, (TextStyle *)this);
	UIManager::styles()->apply(this, (BorderStyle *)this);
	UIManager::styles()->apply(this, (TabViewStyle *)this);
	m_tab_layout = new Grid(parent);
	m_scroll_tab  = new ScrollView(m_tab_layout);
	m_tabs = new Grid(m_scroll_tab);
		if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
		{
			m_scroll_tab->scroll_direction(ScrollDirection::SCROLL_HORIZONTAL);
			m_scroll_tab->align(Align::ALIGN_LEFT);
			m_scroll_tab->cell(0,0);
		}
		else
		{
			m_scroll_tab->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
			m_scroll_tab->align(Align::ALIGN_TOP);
			m_scroll_tab->cell(0,0);

		}
		m_scroll_tab->scrollbar_visible(false);
}

// Destroy the tab view.
TabView::~TabView()
{
}

/** Update tab */
void TabView::update_tabs()
{
	if (m_tab_direction != m_tab_previous_direction)
	{
		if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
		{
			m_scroll_tab->scroll_direction(ScrollDirection::SCROLL_HORIZONTAL);
			m_scroll_tab->align(Align::ALIGN_LEFT);
		}
		else
		{
			m_scroll_tab->scroll_direction(ScrollDirection::SCROLL_VERTICAL);
			m_scroll_tab->align(Align::ALIGN_TOP);
		}
		m_scroll_tab->scrollbar_visible(false);
		m_tab_previous_direction = m_tab_direction;
	}
}


// Create a tab and its page, returning the page owned by this TabView.
Pane * TabView::add_tab(const String & title)
{
	dump(m_tab_layout);
	Pane * result = new Pane(m_tab_layout);
	if (result)
	{
		result->id(FIRST_PAGE+m_tab_id);
		if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
		{
			result->cell(1, 0);
		}
		else
		{
			result->cell(0, 1);
		}
	}
	result->hidden(true);

	Button * tab_title = new Button(m_tabs);
	if (tab_title)
	{
		// tab_title->CommonStyle::set(*this);
		// tab_title->WidgetStyle::set(*this);
		// tab_title->BorderStyle::set(*this);
		// tab_title->TextStyle::set(*this);
		// tab_title->PaddingStyle::set(*this);
		tab_title->text(title);

		tab_title->id(FIRST_TAB+m_tab_id);
		if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
		{
			tab_title->cell(0, m_tab_id++);
		}
		else
		{
			tab_title->cell(m_tab_id++, 0);
		}
	}
	return result;
}


// Serialize the widget and its children.
void TabView::serialize(JsonIterator & it)
{
	it[StyleNames::WIDGET_TYPE] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	PaddingStyle::serialize(it);
	TabViewStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	Widget::serialize(it);
}

// Restore widget styles.
void TabView::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	TabViewStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

// Get the style cascade mode for this widget.
StyleCascadeMode TabView::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

// Place the internal layout in the TabView area.
void TabView::place(const Area & area, bool in_layout)
{
	update_tabs();


	compute_widget_placement(area, in_layout, 0, true);
}

// Handle a focus event
void TabView::on_focus_event(Widget * widget, const FocusEvent & evt)
{
	Widget * window = widget->root();

	if (window)
	{
		uint16_t id;
		for (id = 100; id < 110; id++)
		{
			Widget * page = window->search(id);
			if (page)
			{
				if (widget->id() + 100 == page->id())
				{
					page->hidden(false);
				}
				else
				{
					page->hidden(true);
				}
			}
		}
	}
}
