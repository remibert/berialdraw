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
	UIManager::styles()->apply(this, (TabViewStyle *)this);
	UIManager::styles()->apply(this, (CommonStyle *)this);
	UIManager::styles()->apply(this, (WidgetStyle *)this);
	UIManager::styles()->apply(this, (PaddingStyle *)this);
	UIManager::styles()->apply(this, (TextStyle *)this);
	UIManager::styles()->apply(this, (BorderStyle *)this);
	m_previous_active_tab_style = tab_style(m_tab_direction);
	m_tab_layout = new Grid(this);
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
	m_scroll_tab->margin(tab_style(m_tab_direction).margin());
	m_scroll_tab->scrollbar_visible(false);
	m_focusable = false;
	bind(this, &TabView::on_key_tab);
}

// Destroy the tab view.
TabView::~TabView()
{
}

/** Update tab */
void TabView::update_tabs()
{
	bool direction_changed = m_tab_direction != m_previous_tab_direction;
	const TabStyle & active_style = tab_style(m_tab_direction);
	bool tab_style_changed = active_style != m_previous_active_tab_style;

	if (direction_changed || tab_style_changed)
	{
		update_tab_scroll(direction_changed, tab_style_changed);
		apply_tab_style(active_style);
		m_previous_active_tab_style = active_style;
	}
	if (direction_changed)
	{
		reflow_tabs();
	}
}

// Update the tab bar scroll settings.
void TabView::update_tab_scroll(bool direction_changed, bool tab_style_changed)
{
	if (direction_changed)
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
		m_previous_tab_direction = m_tab_direction;
	}
	if (tab_style_changed)
	{
		m_scroll_tab->margin(tab_style(m_tab_direction).margin());
	}
}

// Apply the active directional style to every existing tab button.
void TabView::apply_tab_style(const TabStyle & style)
{
	for (Dim index = 0; index < m_tab_id; index++)
	{
		Button * title = tab_button(index);
		if (title)
		{
			title->margin(style.inter_margin());
			title->borders(style.borders());
		}
	}
}

// Reposition the existing tabs and pages after a direction change.
void TabView::reflow_tabs()
{
	for (Dim index = 0; index < m_tab_id; index++)
	{
		Button * title = tab_button(index);
		Pane * tab_page = page(index);
		if (title)
		{
			if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
			{
				title->cell(0, index);
			}
			else
			{
				title->cell(index, 0);
			}
		}
		if (tab_page)
		{
			if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
			{
				tab_page->cell(1, 0);
			}
			else
			{
				tab_page->cell(0, 1);
			}
		}
	}
	UIManager::invalidator()->dirty(m_tab_layout, Invalidator::GEOMETRY);
}

// Create a tab and its page, returning the page owned by this TabView.
Pane * TabView::add_tab(const String & title)
{
	Dim index = m_tab_id++;
	Pane * result = new Pane(m_tab_layout);
	result->id(FIRST_PAGE + index);
	if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
	{
		result->cell(1, 0);
	}
	else
	{
		result->cell(0, 1);
	}
	result->hidden(true);
	result->size_policy(SizePolicy::ENLARGE_ALL);

	Button * tab_title = new Button(m_tabs);
	if (tab_title)
	{
		const TabStyle & active_style = tab_style(m_tab_direction);
		// TabView supplies directional layout defaults; callers can still customize each button.
		tab_title->text(title);
		tab_title->id(FIRST_TAB + index);
		tab_title->focusable(false);
		tab_title->margin(active_style.inter_margin());
		tab_title->borders(active_style.borders());
		if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
		{
			tab_title->cell(0, index);
		}
		else
		{
			tab_title->cell(index, 0);
		}
		tab_title->bind(this, &TabView::on_click_tab);
	}
	if (m_selected_index == UNDEFINED_INDEX)
	{
		select_tab(index, false);
	}
	return result;
}

// Remove a tab by its index.
bool TabView::remove_tab(Dim index)
{
	bool result = false;
	if (index < m_tab_id)
	{
		result = true;
		Button * title = tab_button(index);
		Pane * tab_page = page(index);
		bool was_selected = index == m_selected_index;
		bool tab_has_focus = contains_focus(tab_page) || (title && title->focused());
		Dim replacement_index = UNDEFINED_INDEX;

		// Move focus and selection before deleting the active tab.
		if (was_selected && m_tab_id > 1)
		{
			replacement_index = index + 1 < m_tab_id ? index + 1 : index - 1;
			select_tab(replacement_index, tab_has_focus);
		}
		else if (was_selected == false && tab_has_focus && m_selected_index != UNDEFINED_INDEX)
		{
			select_tab(m_selected_index, true);
		}

		// Delete the page and its corresponding tab button.
		if (tab_page)
		{
			delete tab_page;
		}
		if (title)
		{
			delete title;
		}

		// Keep tab ids and grid cells contiguous after the removal.
		for (Dim tab = index + 1; tab < m_tab_id; tab++)
		{
			Button * shifted_title = tab_button(tab);
			Pane * shifted_page = page(tab);
			if (shifted_title)
			{
				shifted_title->id(FIRST_TAB + tab - 1);
				if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
				{
					shifted_title->cell(0, tab - 1);
				}
				else
				{
					shifted_title->cell(tab - 1, 0);
				}
			}
			if (shifted_page)
			{
				shifted_page->id(FIRST_PAGE + tab - 1);
			}
		}
		m_tab_id--;

		// Retain the same selected tab, adjusting its index when necessary.
		if (was_selected)
		{
			if (replacement_index == UNDEFINED_INDEX)
			{
				m_selected_index = UNDEFINED_INDEX;
			}
			else
			{
				m_selected_index = replacement_index > index ? replacement_index - 1 : replacement_index;
			}
		}
		else if (index < m_selected_index)
		{
			m_selected_index--;
		}
		UIManager::invalidator()->dirty(m_tab_layout, Invalidator::GEOMETRY);
	}
	return result;
}

// Remove the first tab with the matching title.
bool TabView::remove_tab(const String & title)
{
	bool result = false;
	for (Dim index = 0; index < m_tab_id && result == false; index++)
	{
		Button * tab_title = tab_button(index);
		if (tab_title && tab_title->text() == title)
		{
			result = remove_tab(index);
		}
	}
	return result;
}

// Get a page by its tab index.
Pane * TabView::page(Dim index) const
{
	Pane * result = 0;
	if (m_tab_layout && index < m_tab_id)
	{
		result = dynamic_cast<Pane *>(m_tab_layout->search(FIRST_PAGE + index));
	}
	return result;
}

// Get the selected page.
Pane * TabView::selected_page() const
{
	return page(m_selected_index);
}

// Get a tab button by its index.
Button * TabView::tab_button(Dim index) const
{
	Button * result = 0;
	if (m_tabs && index < m_tab_id)
	{
		result = dynamic_cast<Button *>(m_tabs->search(FIRST_TAB + index));
	}
	return result;
}

// Select a tab by its index.
bool TabView::select(Dim index)
{
	return select_tab(index, false);
}

// Check whether a widget or one of its descendants has focus.
bool TabView::contains_focus(Widget * widget) const
{
	bool result = false;
	if (widget)
	{
		result = widget->focused();
		Widget * child = widget->children();
		while (child && result == false)
		{
			result = contains_focus(child);
			child = child->next();
		}
	}
	return result;
}

// Select a tab and optionally move focus to its button.
bool TabView::select_tab(Dim index, bool focus_tab)
{
	bool result = false;
	Pane * new_page = page(index);
	Button * new_button = tab_button(index);
	if (new_page && new_button)
	{
		result = true;
		if (index == m_selected_index)
		{
			new_page->hidden(false);
			if (focus_tab && new_button->focused() == false)
			{
				new_button->focus_to(new_button);
			}
		}
		else
		{
			Pane * old_page = selected_page();
			Button * old_button = tab_button(m_selected_index);
			bool move_focus = focus_tab || contains_focus(old_page) ||
				(old_button && old_button->focused());

			if (old_page)
			{
				old_page->hidden(true);
			}
			new_page->hidden(false);
			m_selected_index = index;

			for (Dim tab = 0; tab < m_tab_id; tab++)
			{
				Button * button = tab_button(tab);
				if (button)
				{
					bool selected = tab == index;
					button->focusable(selected);
					button->selected(selected);
				}
			}
			if (move_focus)
			{
				new_button->focus_to(new_button);
			}
			UIManager::invalidator()->dirty(m_tab_layout, Invalidator::GEOMETRY);
		}
	}
	return result;
}

// Handle a click on a tab button.
void TabView::on_click_tab(Widget * widget, const ClickEvent &)
{
	if (widget && widget->id() >= FIRST_TAB)
	{
		Dim index = widget->id() - FIRST_TAB;
		select_tab(index, widget->focused() == false);
	}
}

// Handle keyboard navigation while the active tab button has focus.
void TabView::on_key_tab(Widget *, const KeyEvent & evt)
{
	if (evt.state() == KeyEvent::KEY_DOWN && m_selected_index != UNDEFINED_INDEX && m_tab_id > 1)
	{
		Button * active_button = tab_button(m_selected_index);
		bool previous = false;
		bool next = false;
		if (active_button && active_button->focused())
		{
			if (m_tab_direction == TabDirection::TAB_HORIZONTAL)
			{
				previous = evt.key() == (wchar_t)ReservedKey::KEY_LEFT;
				next = evt.key() == (wchar_t)ReservedKey::KEY_RIGHT;
			}
			else
			{
				previous = evt.key() == (wchar_t)ReservedKey::KEY_UP;
				next = evt.key() == (wchar_t)ReservedKey::KEY_DOWN;
			}
		}
		if (previous || next)
		{
			Dim target = m_selected_index;
			if (previous)
			{
				target = target == 0 ? m_tab_id - 1 : target - 1;
			}
			else
			{
				target = target + 1 == m_tab_id ? 0 : target + 1;
			}
			select_tab(target, true);
		}
	}
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
	focusable(false);
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
	UIManager::invalidator()->dirty(m_tab_layout, Invalidator::GEOMETRY);
	m_tab_layout->layout(m_contentclip, true);
}
