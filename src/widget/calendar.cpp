#include "berialdraw_imp.hpp"

using namespace berialdraw;

namespace
{
	const char * const month_message_ids[] =
	{
		"calendar.month.january", "calendar.month.february", "calendar.month.march",
		"calendar.month.april", "calendar.month.may", "calendar.month.june",
		"calendar.month.july", "calendar.month.august", "calendar.month.september",
		"calendar.month.october", "calendar.month.november", "calendar.month.december"
	};

	const char * const weekday_message_ids[] =
	{
		"calendar.weekday.monday.short", "calendar.weekday.tuesday.short",
		"calendar.weekday.wednesday.short", "calendar.weekday.thursday.short",
		"calendar.weekday.friday.short", "calendar.weekday.saturday.short",
		"calendar.weekday.sunday.short"
	};

	// Add a day offset to a date within the supported calendar range.
	bool offset_date(const CalendarDate & base, int offset, CalendarDate & result)
	{
		bool valid = base.is_valid();
		int year = base.year;
		int month = base.month;
		int day = (int)base.day + offset;

		while (valid && day < 1)
		{
			month--;
			if (month < 1)
			{
				month = 12;
				year--;
			}
			valid = year >= Date::MIN_YEAR;
			if (valid)
			{
				day += Date::days_in_month(year, (uint8_t)month);
			}
		}

		while (valid && day > Date::days_in_month(year, (uint8_t)month))
		{
			day -= Date::days_in_month(year, (uint8_t)month);
			month++;
			if (month > 12)
			{
				month = 1;
				year++;
			}
			valid = year <= Date::MAX_YEAR;
		}

		if (valid)
		{
			result.year = year;
			result.month = (uint8_t)month;
			result.day = (uint8_t)day;
			valid = result.is_valid();
		}
		return valid;
	}

	// Resolve a translation key from the active language.
	String translated_message(const char * id)
	{
		String result(id);
		Language * language = UIManager::language();
		if (language)
		{
			result = language->translate(id);
		}
		return result;
	}

}

// Find a widget by ID within the given widget's descendants.
static Widget * find_widget_by_id(Widget * parent, Dim id)
{
	Widget * result = nullptr;
	Widget * child = parent ? parent->children() : nullptr;

	while (child && result == nullptr)
	{
		result = child->search(id);
		child = child->next();
	}

	return result;
}

// Create a calendar initialized to the local date.
Calendar::Calendar(Widget * parent) :
	Widget(StyleNames::TYPE_CALENDAR, parent, sizeof(Calendar))
{
	if (initialize())
	{
		CalendarDate current;
		if (Date::today(current))
		{
			commit_date(current, false);
			update_controls();
		}
		else
		{
			bd_printf("Calendar: cannot read the local date; set a valid date explicitly\n");
		}
	}
}

// Create a calendar initialized to a specified date.
Calendar::Calendar(Widget * parent, const CalendarDate & date) :
	Widget(StyleNames::TYPE_CALENDAR, parent, sizeof(Calendar))
{
	if (initialize())
	{
		if (!commit_date(date, false))
		{
			bd_printf("Calendar: invalid initial date\n");
		}
		else
		{
			update_controls();
		}
	}
}

// Destroy the calendar widget.
Calendar::~Calendar()
{
}

// Initialize styles and child widgets.
bool Calendar::initialize()
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
	UIManager::styles()->apply(this, (CalendarStyle*)this);
	bool result = UIManager::styles()->apply_widget_layout(this);
	if (result)
	{
		result = build_controls();
	}
	if (result)
	{
		bind(this, &Calendar::on_date_cell_key);
	}
	else
	{
		bd_printf("Calendar: unable to load controls from calendar.sty\n");
	}
	return result;
}

// Build the calendar controls and fixed-capacity date grid.
bool Calendar::build_controls()
{
	bool result = true;
	m_content = dynamic_cast<Column*>(find_widget_by_id(this, CONTENT_ID));
	Row * controls = dynamic_cast<Row*>(find_widget_by_id(this, CONTROLS_ID));
	m_previous_button = dynamic_cast<Button*>(find_widget_by_id(this, PREVIOUS_BUTTON_ID));
	m_month_combo = dynamic_cast<Combo*>(find_widget_by_id(this, MONTH_COMBO_ID));
	m_year_combo = dynamic_cast<Combo*>(find_widget_by_id(this, YEAR_COMBO_ID));
	m_next_button = dynamic_cast<Button*>(find_widget_by_id(this, NEXT_BUTTON_ID));
	m_today_button = dynamic_cast<Button*>(find_widget_by_id(this, TODAY_BUTTON_ID));
	m_table = dynamic_cast<TableView*>(find_widget_by_id(this, TABLE_ID));

	if (m_content == nullptr || controls == nullptr || m_previous_button == nullptr ||
		m_month_combo == nullptr || m_year_combo == nullptr || m_next_button == nullptr ||
		m_today_button == nullptr || m_table == nullptr)
	{
		result = false;
	}

	for (Dim column = 0; result && column < 7; column++)
	{
		Widget * weekday = find_widget_by_id(m_table, WEEKDAY_BUTTON_ID_BASE + column);
		if (dynamic_cast<Button*>(weekday) == nullptr)
		{
			result = false;
		}
	}

	for (Dim index = 0; result && index < 49; index++)
	{
		m_day_buttons[index] = dynamic_cast<Button*>(find_widget_by_id(m_table, DAY_BUTTON_ID_BASE + index));
		if (m_day_buttons[index] == nullptr)
		{
			result = false;
		}
	}

	if (result)
	{
		m_previous_button->bind(this, &Calendar::on_previous_month);
		m_month_combo->bind(this, &Calendar::on_month_key);
		m_year_combo->bind(this, &Calendar::on_year_key);
		m_next_button->bind(this, &Calendar::on_next_month);
		m_today_button->bind(this, &Calendar::on_today);
		m_today_button->text(translated_message("calendar.action.today"));
		update_table_grid_style();

		for (Dim column = 0; column < 7; column++)
		{
			Button * weekday = dynamic_cast<Button*>(find_widget_by_id(m_table, WEEKDAY_BUTTON_ID_BASE + column));
			update_weekday_style(weekday);
			weekday->text(translated_message(weekday_message_ids[column]));
		}

		for (Dim index = 0; index < 49; index++)
		{
			Button * day = m_day_buttons[index];
			update_day_style(day, false, false);
			day->bind(this, &Calendar::on_date_cell_click);
		}
	}
	else
	{
		m_table = nullptr;
	}

	return result;
}

// Return the currently selected date.
const CalendarDate & Calendar::date() const
{
	return m_date;
}

// Select a valid date.
bool Calendar::date(const CalendarDate & value)
{
	bool result = commit_date(value, true);
	if (result)
	{
		update_controls();
	}
	return result;
}

// Return the selected year.
int Calendar::year() const
{
	return m_date.year;
}

// Select a year while preserving the month and clamping the day.
bool Calendar::year(int value)
{
	bool result = false;
	if (m_date.is_valid() && value >= Date::MIN_YEAR && value <= Date::MAX_YEAR)
	{
		CalendarDate updated = m_date;
		updated.year = value;
		uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
		if (updated.day > maximum_day)
		{
			updated.day = maximum_day;
		}
		result = commit_date(updated, true);
		if (result)
		{
			update_controls();
		}
	}
	else
	{
		bd_printf("Calendar: invalid year\n");
	}
	return result;
}

// Return the selected month.
uint8_t Calendar::month() const
{
	return m_date.month;
}

// Select a month while preserving the year and clamping the day.
bool Calendar::month(uint8_t value)
{
	bool result = false;
	if (m_date.is_valid() && value >= 1 && value <= 12)
	{
		CalendarDate updated = m_date;
		updated.month = value;
		uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
		if (updated.day > maximum_day)
		{
			updated.day = maximum_day;
		}
		result = commit_date(updated, true);
		if (result)
		{
			update_controls();
		}
	}
	else
	{
		bd_printf("Calendar: invalid month\n");
	}
	return result;
}

// Return the selected day.
uint8_t Calendar::day() const
{
	return m_date.day;
}

// Select a valid day in the current month.
bool Calendar::day(uint8_t value)
{
	bool result = false;
	if (m_date.is_valid() && value >= 1 && value <= Date::days_in_month(m_date.year, m_date.month))
	{
		CalendarDate updated = m_date;
		updated.day = value;
		result = commit_date(updated, true);
		if (result)
		{
			update_controls();
		}
	}
	else
	{
		bd_printf("Calendar: invalid day\n");
	}
	return result;
}

// Show and select the previous month.
void Calendar::previous_month()
{
	bool valid = m_date.is_valid();
	CalendarDate updated = m_date;
	if (valid)
	{
		if (updated.month == 1)
		{
			if (updated.year == Date::MIN_YEAR)
			{
				valid = false;
			}
			else
			{
				updated.year--;
				updated.month = 12;
			}
		}
		else
		{
			updated.month--;
		}
	}
	if (valid)
	{
		uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
		if (updated.day > maximum_day)
		{
			updated.day = maximum_day;
		}
		commit_date(updated, true);
		update_controls();
	}
	else
	{
		bd_printf("Calendar: previous month is outside the supported date range\n");
	}
}

// Show and select the next month.
void Calendar::next_month()
{
	bool valid = m_date.is_valid();
	CalendarDate updated = m_date;
	if (valid)
	{
		if (updated.month == 12)
		{
			if (updated.year == Date::MAX_YEAR)
			{
				valid = false;
			}
			else
			{
				updated.year++;
				updated.month = 1;
			}
		}
		else
		{
			updated.month++;
		}
	}
	if (valid)
	{
		uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
		if (updated.day > maximum_day)
		{
			updated.day = maximum_day;
		}
		commit_date(updated, true);
		update_controls();
	}
	else
	{
		bd_printf("Calendar: next month is outside the supported date range\n");
	}
}

// Select the local date and show its month.
void Calendar::today()
{
	CalendarDate current;
	if (Date::today(current))
	{
		commit_date(current, true);
		update_controls();
	}
	else
	{
		bd_printf("Calendar: cannot read the local date\n");
	}
}

// Serialize calendar styles and the selected date.
void Calendar::serialize(JsonIterator & it)
{
	it[StyleNames::WIDGET_TYPE] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	PaddingStyle::serialize(it);
	CalendarStyle::serialize(it);
	it[StyleNames::CALENDAR_DATE_YEAR] = m_date.year;
	it[StyleNames::CALENDAR_DATE_MONTH] = (unsigned int)m_date.month;
	it[StyleNames::CALENDAR_DATE_DAY] = (unsigned int)m_date.day;
}

// Restore calendar styles and the selected date.
void Calendar::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	CalendarStyle::unserialize(it);

	bool has_year = it[StyleNames::CALENDAR_DATE_YEAR].exist();
	bool has_month = it[StyleNames::CALENDAR_DATE_MONTH].exist();
	bool has_day = it[StyleNames::CALENDAR_DATE_DAY].exist();
	if (has_year || has_month || has_day)
	{
		CalendarDate restored;
		restored.year = (int)(it[StyleNames::CALENDAR_DATE_YEAR] | 0);
		restored.month = (uint8_t)(it[StyleNames::CALENDAR_DATE_MONTH] | 0);
		restored.day = (uint8_t)(it[StyleNames::CALENDAR_DATE_DAY] | 0);
		if (has_year && has_month && has_day && restored.is_valid())
		{
			commit_date(restored, false);
		}
		else
		{
			bd_printf("Calendar: invalid serialized date\n");
		}
	}
	update_controls();
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

// Return the calendar style cascade mode.
StyleCascadeMode Calendar::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}

// Place the calendar and its content column.
void Calendar::place(const Area & area, bool in_layout)
{
	if (UIManager::invalidator()->is_dirty(this))
	{
		compute_widget_placement(area, in_layout, 0, false);
		Area backclip(m_backclip);
		Area foreclip(m_foreclip);
		Area contentclip(m_contentclip);
		if (m_content)
		{
			Widget::place(m_contentclip, in_layout);
		}
		m_backclip = backclip;
		m_foreclip = foreclip;
		m_contentclip = contentclip;
		UIManager::invalidator()->undirty(this, Invalidator::GEOMETRY);
	}
}

// Commit a valid date, redraw the grid, and optionally emit a change event.
bool Calendar::commit_date(const CalendarDate & value, bool notify)
{
	bool result = false;
	if (value.is_valid())
	{
		bool changed = m_date != value;
		if (changed)
		{
			m_date = value;
			update_date_grid();
			if (notify)
			{
				UIManager::notifier()->push_event(new CalendarEvent(m_date, this));
			}
		}
		result = true;
	}
	return result;
}

// Parse a complete year value from the editable year combo.
bool Calendar::parse_year(const String & value, int & year) const
{
	bool result = false;
	if (value.size() > 0)
	{
		char * end = nullptr;
		long parsed = strtol(value.c_str(), &end, 10);
		if (end != value.c_str() && *end == '\0' &&
			parsed >= Date::MIN_YEAR && parsed <= Date::MAX_YEAR)
		{
			year = (int)parsed;
			result = true;
		}
	}
	return result;
}

// Update all translated controls, combo choices, and visible date cells.
void Calendar::update_controls()
{
	if (m_date.is_valid() && m_month_combo && m_year_combo)
	{
		if (m_today_button)
		{
			m_today_button->text(translated_message("calendar.action.today"));
		}
		m_month_combo->clear();
		for (int index = 0; index < 12; index++)
		{
			ListItem * item = m_month_combo->append(translated_message(month_message_ids[index]));
			if (item)
			{
				item->bind(this, &Calendar::on_month_selection);
			}
		}
		m_month_combo->select((int)m_date.month - 1);
		update_year_combo();
		update_date_grid();
	}
}

// Populate editable year suggestions around the selected year.
void Calendar::update_year_combo()
{
	if (m_date.is_valid() && m_year_combo)
	{
		m_year_combo->clear();
		int first_year = m_date.year > Date::MIN_YEAR + 10 ? m_date.year - 10 : Date::MIN_YEAR;
		int last_year = m_date.year < Date::MAX_YEAR - 10 ? m_date.year + 10 : Date::MAX_YEAR;
		int selected_index = 0;
		int index = 0;
		for (int year = first_year; year <= last_year; year++)
		{
			String text;
			text.print("%d", year);
			ListItem * item = m_year_combo->append(text);
			if (item)
			{
				item->bind(this, &Calendar::on_year_selection);
			}
			if (year == m_date.year)
			{
				selected_index = index;
			}
			index++;
		}
		m_year_combo->select(selected_index);
	}
}

// Apply shared table grid settings.
void Calendar::update_table_grid_style()
{
	if (m_table)
	{
		m_table->grid_color(grid_color());
		m_table->grid_visible(grid_thickness() > 0);
		m_table->horizontal_thickness(grid_thickness());
		m_table->vertical_thickness(grid_thickness());
	}
}

// Apply the weekday header appearance.
void Calendar::update_weekday_style(Button * weekday)
{
	if (weekday)
	{
		weekday->color(weekday_back_color());
		weekday->text_color(weekday_text_color());
		weekday->font_size(weekday_font_size());
	}
}

// Apply the appearance for a date cell according to its month and selection state.
void Calendar::update_day_style(Button * day, bool outside_month, bool selected)
{
	if (day)
	{
		day->border_color(day_border_color());
		day->thickness(day_border_thickness());
		day->font_size(day_font_size());
		day->color(selected ? day_selected_back_color() :
			(outside_month ? outside_day_back_color() : day_back_color()));
		day->text_color(selected ? day_selected_text_color() :
			(outside_month ? outside_day_text_color() : day_text_color()));
	}
}

// Update the fixed-capacity weekday and date grid.
void Calendar::update_date_grid()
{
	if (m_table && m_date.is_valid())
	{
		update_table_grid_style();
		m_table->alternating_row_color1(Color::TRANSPARENT);
		m_table->alternating_row_color2(Color::TRANSPARENT);
		m_table->border_color(grid_color());
		m_table->thickness(0);

		for (Dim column = 0; column < 7; column++)
		{
			Button * weekday = dynamic_cast<Button*>(m_table->get_widget(0, column));
			if (weekday)
			{
				update_weekday_style(weekday);
			}
		}

		CalendarDate first_day = m_date;
		first_day.day = 1;
		uint8_t weekday = Date::weekday_monday(first_day);
		uint8_t month_days = Date::days_in_month(m_date.year, m_date.month);
		int month_weeks = ((int)weekday + month_days + 6) / 7;
		int visible_weeks = month_weeks;
		if (visible_weeks < 6)
		{
			visible_weeks = 6;
		}
		m_visible_weeks = (Dim)visible_weeks;
		int grid_start_offset = 0 - (int)weekday;
		int trailing_days = (visible_weeks * 7) - ((int)weekday + (int)month_days);
		if (trailing_days > 7)
		{
			grid_start_offset -= 7;
		}

		for (Dim cell_index = 0; cell_index < 49; cell_index++)
		{
			Button * cell = m_day_buttons[cell_index];
			Dim week = cell_index / 7;
			Dim column = cell_index % 7;
			if ((int)week < visible_weeks)
			{
				CalendarDate cell_date;
				int offset = (int)cell_index + grid_start_offset;
				bool valid = offset_date(first_day, offset, cell_date);
				m_cell_date_valid[cell_index] = valid;
				if (valid)
				{
					m_cell_dates[cell_index] = cell_date;
					String day_text;
					day_text.print("%u", (unsigned int)cell_date.day);
					cell->text(day_text);
					bool outside_month = cell_date.year != m_date.year || cell_date.month != m_date.month;
					bool selected = cell_date == m_date;
					update_day_style(cell, outside_month, selected);
					cell->enabled(true);
				}
				else
				{
					m_cell_dates[cell_index] = CalendarDate();
					cell->text("");
					update_day_style(cell, true, false);
					cell->color(Color::TRANSPARENT);
					cell->enabled(false);
				}
				cell->hidden(false);
				cell->row(week + 1);
				cell->column(column);
			}
			else
			{
				m_cell_dates[cell_index] = CalendarDate();
				m_cell_date_valid[cell_index] = false;
				cell->text("");
				cell->enabled(false);
				cell->hidden(true);
				cell->row(UNDEFINED_CELL);
				cell->column(column);
			}
		}

		UIManager::invalidator()->dirty(m_table, Invalidator::GEOMETRY);
	}
}

// Navigate to the previous month when its button is clicked.
void Calendar::on_previous_month(Widget * widget, const ClickEvent & event)
{
	(void)widget;
	(void)event;
	previous_month();
}

// Show the next month after its navigation button is clicked.
void Calendar::on_next_month(Widget * widget, const ClickEvent & event)
{
	(void)widget;
	(void)event;
	next_month();
}

// Select today's date after its button is clicked.
void Calendar::on_today(Widget * widget, const ClickEvent & event)
{
	(void)widget;
	(void)event;
	today();
}

// Select a month chosen from the month combo.
void Calendar::on_month_selection(Widget * widget, const ClickEvent & event)
{
	(void)event;
	ListItem * clicked_item = dynamic_cast<ListItem*>(widget);
	int selected_index = -1;
	for (int index = 0; clicked_item && index < (int)m_month_combo->count(); index++)
	{
		if (m_month_combo->at(index) == clicked_item)
		{
			selected_index = index;
		}
	}
	if (selected_index >= 0 && m_date.is_valid())
	{
		CalendarDate updated = m_date;
		updated.month = (uint8_t)(selected_index + 1);
		uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
		if (updated.day > maximum_day)
		{
			updated.day = maximum_day;
		}
		commit_date(updated, true);
	}
}

// Select a year chosen from the year combo.
void Calendar::on_year_selection(Widget * widget, const ClickEvent & event)
{
	(void)event;
	int year = 0;
	ListItem * clicked_item = dynamic_cast<ListItem*>(widget);
	String value = clicked_item ? clicked_item->text() : m_year_combo->input();
	if (m_date.is_valid() && parse_year(value, year))
	{
		CalendarDate updated = m_date;
		updated.year = year;
		uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
		if (updated.day > maximum_day)
		{
			updated.day = maximum_day;
		}
		commit_date(updated, true);
	}
}

// Apply month combo changes made with the keyboard.
void Calendar::on_month_key(Widget * widget, const KeyEvent & event)
{
	if (widget == m_month_combo && event.state() == KeyEvent::KEY_UP && m_date.is_valid())
	{
		int selected_index = m_month_combo->selected_index();
		if (selected_index >= 0)
		{
			CalendarDate updated = m_date;
			updated.month = (uint8_t)(selected_index + 1);
			uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
			if (updated.day > maximum_day)
			{
				updated.day = maximum_day;
			}
			commit_date(updated, true);
		}
	}
}

// Commit typed years when Enter is pressed.
void Calendar::on_year_key(Widget * widget, const KeyEvent & event)
{
	bool arrow_released = event.state() == KeyEvent::KEY_UP &&
		(event.key() == (wchar_t)ReservedKey::KEY_UP ||
		 event.key() == (wchar_t)ReservedKey::KEY_DOWN);
	if (widget == m_year_combo && m_date.is_valid() && arrow_released)
	{
		ListItem * selected = m_year_combo->selected_item();
		String value = selected ? selected->text() : m_year_combo->input();
		int selected_year = 0;
		if (parse_year(value, selected_year))
		{
			year(selected_year);
		}
	}
	else if (widget == m_year_combo && event.state() == KeyEvent::KEY_DOWN &&
		event.key() == (wchar_t)ReservedKey::KEY_ENTER)
	{
		int year = 0;
		if (m_date.is_valid() && parse_year(m_year_combo->input(), year))
		{
			CalendarDate updated = m_date;
			updated.year = year;
			uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
			if (updated.day > maximum_day)
			{
				updated.day = maximum_day;
			}
			if (commit_date(updated, true))
			{
				update_controls();
			}
		}
		else
		{
			bd_printf("Calendar: invalid year input\n");
			update_controls();
		}
	}
}

// Select the date represented by a clicked calendar cell.
void Calendar::on_date_cell_click(Widget * widget, const ClickEvent & event)
{
	(void)event;
	bool found = false;
	for (Dim index = 0; index < m_visible_weeks * 7 && !found; index++)
	{
		if (m_day_buttons[index] == widget && m_cell_date_valid[index])
		{
			found = true;
			commit_date(m_cell_dates[index], true);
			update_controls();
		}
	}
}

// Navigate the selected date with the keyboard.
void Calendar::on_date_cell_key(Widget * widget, const KeyEvent & event)
{
	int offset = 0;
	int month_offset = 0;
	uint8_t target_day = 0;
	bool date_cell_focused = false;
	for (Dim index = 0; index < m_visible_weeks * 7 && !date_cell_focused; index++)
	{
		date_cell_focused = m_day_buttons[index] && m_day_buttons[index]->focused();
	}
	if (widget == this && date_cell_focused && event.state() == KeyEvent::KEY_DOWN && m_date.is_valid())
	{
		switch (event.key())
		{
		case (wchar_t)ReservedKey::KEY_RIGHT:
			offset = 1;
			break;
		case (wchar_t)ReservedKey::KEY_LEFT:
			offset = -1;
			break;
		case (wchar_t)ReservedKey::KEY_DOWN:
			offset = 7;
			break;
		case (wchar_t)ReservedKey::KEY_UP:
			offset = -7;
			break;
		case (wchar_t)ReservedKey::KEY_PAGE_UP:
			month_offset = -1;
			break;
		case (wchar_t)ReservedKey::KEY_PAGE_DOWN:
			month_offset = 1;
			break;
		case (wchar_t)ReservedKey::KEY_HOME:
			target_day = 1;
			break;
		case (wchar_t)ReservedKey::KEY_END:
			target_day = Date::days_in_month(m_date.year, m_date.month);
			break;
		default:
			break;
		}

		CalendarDate updated;
		updated = m_date;
		bool valid = true;
		if (offset != 0)
		{
			valid = offset_date(m_date, offset, updated);
		}
		else if (month_offset != 0)
		{
			if (month_offset < 0)
			{
				if (updated.month == 1)
				{
					if (updated.year == Date::MIN_YEAR)
					{
						valid = false;
					}
					else
					{
						updated.year--;
						updated.month = 12;
					}
				}
				else
				{
					updated.month--;
				}
			}
			else if (updated.month == 12)
			{
				if (updated.year == Date::MAX_YEAR)
				{
					valid = false;
				}
				else
				{
					updated.year++;
					updated.month = 1;
				}
			}
			else
			{
				updated.month++;
			}

			if (valid)
			{
				uint8_t maximum_day = Date::days_in_month(updated.year, updated.month);
				if (updated.day > maximum_day)
				{
					updated.day = maximum_day;
				}
			}
		}
		else if (target_day != 0)
		{
			updated.day = target_day;
		}

		if (valid && (offset != 0 || month_offset != 0 || target_day != 0) &&
			commit_date(updated, true))
		{
			update_controls();
			bool focus_found = false;
			for (Dim index = 0; index < m_visible_weeks * 7 && !focus_found; index++)
			{
				if (m_cell_date_valid[index] && m_cell_dates[index] == m_date)
				{
					m_day_buttons[index]->focus_to(m_day_buttons[index]);
					focus_found = true;
				}
			}
		}
	}
}
