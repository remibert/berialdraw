#include "berialdraw_imp.hpp"

using namespace berialdraw;

Combo::Combo(Widget * parent):
	Edit(parent)
{
	m_classname = "combo";
	m_edited = 0;
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);
	UIManager::styles()->apply(this, (EditStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
	UIManager::styles()->apply(this, (ComboStyle*)this);
	bind(this, &Combo::on_key);

	m_editable_typing_mode = typing_mode();
	update_editable_state();

	m_button = new Button(this);
	UIManager::styles()->apply("combo_button", (CommonStyle*)m_button);
	UIManager::styles()->apply("combo_button", (WidgetStyle*)m_button);
	UIManager::styles()->apply("combo_button", (BorderStyle*)m_button);
	UIManager::styles()->apply("combo_button", (TextStyle*)m_button);
	UIManager::styles()->apply("combo_button", (PaddingStyle*)m_button);
	m_button->text("V");
	m_button->margin(0);
	m_button->extend(Extend::EXTEND_ALL);
	m_button->bind(this, &Combo::on_button_click);

	Window * window = dynamic_cast<Window*>(root());
	if (window)
	{
		m_popup = new Popup(window);
		m_list = new List(m_popup);
		m_list->extend(Extend::EXTEND_ALL);
		m_list->margin(0);
		m_list->thickness(0);
		m_list->scroll_direction(SCROLL_VERTICAL);
		m_list->selection_mode(ListSelectionMode::LIST_SINGLE_SELECTION);
		m_list->focusable(true);
	}
}

Combo::~Combo()
{
	if (m_popup)
	{
		m_popup->close();
	}
}

void Combo::editable(bool state)
{
	if (ComboStyle::editable() != state)
	{
		if (state == false && typing_mode() != Entry::TypingMode::READ_ONLY)
		{
			m_editable_typing_mode = typing_mode();
		}
		ComboStyle::editable(state);
	}
	update_editable_state();
}

void Combo::update_editable_state()
{
	if (ComboStyle::editable())
	{
		if (typing_mode() == Entry::TypingMode::READ_ONLY)
		{
			typing_mode(m_editable_typing_mode);
		}
	}
	else
	{
		if (typing_mode() != Entry::TypingMode::READ_ONLY)
		{
			m_editable_typing_mode = typing_mode();
			typing_mode(Entry::TypingMode::READ_ONLY);
		}
	}
}

bool Combo::open()
{
	bool result = false;
	if (m_popup && m_list)
	{
		if (m_list->selected_item() && selected_item() == 0)
		{
			m_list->unselect_all();
		}
		Size minimum_size;
		minimum_size.set_q6(m_foreclip.width_q6(), 0);
		m_popup->min_size(minimum_size);

		Size maximum_size = m_popup->max_size();
		maximum_size.height_q6(max_popup_height() == 0 ? Size::MAX_SIZE : (Dim)max_popup_height() << 6);
		m_popup->max_size(maximum_size);
		result = m_popup->open(this);
		if (result)
		{
			Window * window = dynamic_cast<Window*>(root());
			if (window)
			{
				window->focus_to(m_list);
			}
		}
	}
	return result;
}

void Combo::close()
{
	if (m_popup && m_popup->is_open())
	{
		m_popup->close();
		Window * window = dynamic_cast<Window*>(root());
		if (window)
		{
			window->focus_to(this);
		}
	}
}

bool Combo::is_open() const
{
	return m_popup && m_popup->is_open();
}

void Combo::set_value(const String & value)
{
	text(value);
	cursor_position((int32_t)input().count());
}

ListItem * Combo::new_item(int index)
{
	ListItem * result = 0;
	if (m_list)
	{
		result = m_list->new_item(index);
		result->bind(this, &Combo::on_item_click);
	}
	return result;
}

ListItem * Combo::append(const String & text)
{
	return insert(UNDEFINED_INDEX, text);
}

ListItem * Combo::append(std::function<void(ListItem*)> config)
{
	return insert(UNDEFINED_INDEX, config);
}

ListItem * Combo::prepend(const String & text)
{
	return insert(0, text);
}

ListItem * Combo::prepend(std::function<void(ListItem*)> config)
{
	return insert(0, config);
}

ListItem * Combo::insert(int index, const String & text)
{
	bool set_default_value = is_empty() && input().size() == 0;
	ListItem * result = new_item(index);
	if (result)
	{
		result->text(text);
		if (set_default_value)
		{
			set_value(result->text());
		}
	}
	return result;
}

ListItem * Combo::insert(int index, std::function<void(ListItem*)> config)
{
	bool set_default_value = is_empty() && input().size() == 0;
	ListItem * result = new_item(index);
	if (result && config)
	{
		config(result);
	}
	if (result && set_default_value)
	{
		set_value(result->text());
	}
	return result;
}

ListItem * Combo::at(int index) const
{
	return m_list ? m_list->at(index) : 0;
}

ListItem * Combo::operator[](int index) const
{
	return at(index);
}

size_t Combo::count() const
{
	return m_list ? m_list->count() : 0;
}

bool Combo::is_empty() const
{
	return count() == 0;
}

void Combo::select(int index)
{
	if (m_list)
	{
		m_list->select(index);
		ListItem * item = m_list->selected_item();
		if (item)
		{
			set_value(item->text());
		}
	}
}

ListItem * Combo::select_next()
{
	ListItem * result = m_list ? m_list->select_next() : 0;
	if (result)
	{
		set_value(result->text());
	}
	return result;
}

ListItem * Combo::select_previous()
{
	ListItem * result = m_list ? m_list->select_previous() : 0;
	if (result)
	{
		set_value(result->text());
	}
	return result;
}

ListItem * Combo::select_first()
{
	ListItem * result = m_list ? m_list->select_first() : 0;
	if (result)
	{
		set_value(result->text());
	}
	return result;
}

ListItem * Combo::select_last()
{
	ListItem * result = m_list ? m_list->select_last() : 0;
	if (result)
	{
		set_value(result->text());
	}
	return result;
}

void Combo::unselect(int index)
{
	if (m_list)
	{
		m_list->unselect(index);
	}
}

void Combo::unselect_all()
{
	if (m_list)
	{
		m_list->unselect_all();
	}
}

ListItem * Combo::selected_item() const
{
	ListItem * result = m_list ? m_list->selected_item() : 0;
	if (result && result->text() == input())
	{
		return result;
	}
	return 0;
}

int Combo::selected_index() const
{
	int result = -1;
	ListItem * selected = selected_item();
	for (int index = 0; selected && index < (int)count(); index++)
	{
		if (at(index) == selected)
		{
			result = index;
			break;
		}
	}
	return result;
}

Vector<ListItem*> Combo::selected_items() const
{
	return m_list ? m_list->selected_items() : Vector<ListItem*>();
}

Vector<ListItem*> Combo::items() const
{
	return m_list ? m_list->items() : Vector<ListItem*>();
}

void Combo::remove(int index)
{
	if (m_list)
	{
		ListItem * item = at(index);
		bool selected = item && item == selected_item();
		m_list->remove(index);
		if (selected)
		{
			set_value(String());
		}
	}
}

void Combo::clear()
{
	if (m_list)
	{
		m_list->clear();
		set_value(String());
	}
}

void Combo::serialize(JsonIterator & it)
{
	Edit::serialize(it);
	ComboStyle::serialize(it);
	it[StyleNames::WIDGET_TYPE] = m_classname;
}

void Combo::unserialize(JsonIterator & it)
{
	Edit::unserialize(it);
	ComboStyle::unserialize(it);
	update_editable_state();
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Combo::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

Size Combo::content_size()
{
	Size result = Edit::content_size();
	result.width_q6(result.width_q6() + ((Dim)button_width() << 6));
	return result;
}

void Combo::place(const Area & area, bool in_layout)
{
	compute_widget_placement(area, in_layout, m_thickness);
	Dim button_width_q6 = (Dim)button_width() << 6;
	if (button_width_q6 > m_contentclip.width_q6())
	{
		button_width_q6 = m_contentclip.width_q6();
	}
	m_contentclip.width_q6(m_contentclip.width_q6() - button_width_q6);
	if (m_contentclip.width_q6() > m_text_size.width_q6())
	{
		m_contentclip.width_q6(m_text_size.width_q6());
	}
	if (m_contentclip.height_q6() > m_text_size.height_q6())
	{
		m_contentclip.height_q6(m_text_size.height_q6());
	}
	Margin margin;
	place_in_layout(m_contentclip, m_text_size, margin, EXTEND_NONE, m_text_foreclip, m_text_align);

	if (m_button)
	{
		Area button_area(m_foreclip);
		Dim actual_button_width = (Dim)button_width() << 6;
		if (actual_button_width > button_area.width_q6())
		{
			actual_button_width = button_area.width_q6();
		}
		m_button->position_q6((Coord)(m_foreclip.width_q6() - actual_button_width), 0);
		m_button->size_q6(actual_button_width, m_foreclip.height_q6());
		Widget::place_children(m_foreclip, false);
	}
}

Widget * Combo::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;
	if (m_button && m_button->hidden() == false)
	{
		Region button_region(parent_region);
		button_region.intersect(m_button->foreclip());
		if (button_region.is_inside(position) != Overlap::OUT)
		{
			result = m_button;
		}
	}
	if (result == 0)
	{
		result = Edit::hovered(parent_region, position);
	}
	return result;
}

// Handle keyboard navigation and commit editable text
void Combo::on_key(Widget * widget, const KeyEvent & evt)
{
	(void)widget;
	if (m_focused && is_open() == false && evt.state() == KeyEvent::KEY_DOWN)
	{
		if (m_list && m_list->selected_item() && selected_item() == 0)
		{
			m_list->unselect_all();
		}
		if (evt.key() == (wchar_t)ReservedKey::KEY_UP)
		{
			select_previous();
		}
		else if (evt.key() == (wchar_t)ReservedKey::KEY_DOWN)
		{
			select_next();
		}
		else if (evt.key() == (wchar_t)ReservedKey::KEY_ENTER && editable())
		{
			int existing_index = -1;
			if (input().size() > 0)
			{
				for (int index = 0; index < (int)count(); index++)
				{
					if (at(index)->text() == input())
					{
						existing_index = index;
						break;
					}
				}

				if (existing_index >= 0)
				{
					select(existing_index);
				}
				else
				{
					append(input());
					select((int)count() - 1);
				}
			}
			else
			{
				unselect_all();
			}
		}
	}
}

void Combo::on_button_click(Widget * widget, const ClickEvent & evt)
{
	(void)widget;
	(void)evt;
	if (is_open())
	{
		close();
	}
	else
	{
		open();
	}
}

void Combo::on_item_click(Widget * widget, const ClickEvent & evt)
{
	(void)evt;
	ListItem * item = dynamic_cast<ListItem*>(widget);
	if (item)
	{
		set_value(item->text());
		close();
	}
}