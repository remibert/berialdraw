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

// Create a calendar initialized to the local date.
Calendar::Calendar(Widget * parent) :
	Widget("calendar", parent, sizeof(Calendar))
{
	initialize();
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

// Create a calendar initialized to a specified date.
Calendar::Calendar(Widget * parent, const CalendarDate & date) :
	Widget("calendar", parent, sizeof(Calendar))
{
	initialize();
	if (!commit_date(date, false))
	{
		bd_printf("Calendar: invalid initial date\n");
	}
	else
	{
		update_controls();
	}
}

// Destroy the calendar widget.
Calendar::~Calendar()
{
}

// Initialize styles and child widgets.
void Calendar::initialize()
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (PaddingStyle*)this);
	UIManager::styles()->apply(this, (CalendarStyle*)this);
	build_controls();
	bind(this, &Calendar::on_date_cell_key);
}

// Build the calendar controls and fixed-capacity date grid.
void Calendar::build_controls()
{
	m_content = new Column(this);

	Row * controls = new Row(m_content);
	controls->size_policy(SizePolicy::SHRINK_HEIGHT);

	m_previous_button = new Button(controls);
		m_previous_button->text("<");
		m_previous_button->bind(this, &Calendar::on_previous_month);
		m_previous_button->radius(3);
		m_previous_button->focusable(false);

	m_day_combo = new Combo(controls);
		m_day_combo->editable(false);
		m_day_combo->bind(this, &Calendar::on_day_month_key);

	m_month_combo = new Combo(controls);
		m_month_combo->editable(false);
		m_month_combo->bind(this, &Calendar::on_day_month_key);

	m_year_combo = new Combo(controls);
		m_year_combo->editable(true);
		m_year_combo->bind(this, &Calendar::on_year_key);

	m_next_button = new Button(controls);
		m_next_button->text(">");
		m_next_button->bind(this, &Calendar::on_next_month);
		m_next_button->radius(3);
		m_next_button->focusable(false);

	m_today_button = new Button(controls);
		m_today_button->text(translated_message("calendar.action.today"));

	m_today_button->bind(this, &Calendar::on_today);

	m_table = new TableView(m_content);
		m_table->margin(0);
		m_table->grid_color(grid_color());
		m_table->grid_visible(grid_thickness() > 0);
		m_table->horizontal_thickness(grid_thickness());
		m_table->vertical_thickness(grid_thickness());
		m_table->alternating_row_color1(Color::RED);
		m_table->alternating_row_color2(Color::TRANSPARENT);
		m_table->border_color(Color::TRANSPARENT);
		m_table->radius(0);

	for (Dim column = 0; column < 7; column++)
	{
		Button * weekday = new Button(m_table);
			weekday->extend(Extend::EXTEND_ALL);
			weekday->align(Align::CENTER);
			weekday->text_align(Align::CENTER);
			weekday->radius(0);
			weekday->border_color(Color::TRANSPARENT);
			weekday->thickness(0);
			weekday->margin(0);
			weekday->focusable(false);
			weekday->pressable(false);
			weekday->color(weekday_back_color());
			weekday->text_color(weekday_text_color());
			weekday->font_size(weekday_font_size());
			weekday->text(translated_message(weekday_message_ids[column]));
		m_table->set_widget(0, column, weekday);
	}

	for (Dim index = 0; index < 49; index++)
	{
		Button * day = new Button(m_table);
			day->margin(0);
			day->extend(Extend::EXTEND_ALL);
			day->align(Align::CENTER);
			day->text_align(Align::CENTER);
			day->radius(0);
			day->border_color(day_border_color());
			day->thickness(day_border_thickness());
			day->color(day_back_color());
			day->text_color(day_text_color());
			day->font_size(day_font_size());
			day->hidden(true);
			day->bind(this, &Calendar::on_date_cell_click);
		m_table->set_widget((index / 7) + 1, index % 7, day);
		m_day_buttons[index] = day;
	}
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
	it["year"] = m_date.year;
	it["month"] = (unsigned int)m_date.month;
	it["day"] = (unsigned int)m_date.day;
}

// Restore calendar styles and the selected date.
void Calendar::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	PaddingStyle::unserialize(it);
	CalendarStyle::unserialize(it);

	bool has_year = it["year"].exist();
	bool has_month = it["month"].exist();
	bool has_day = it["day"].exist();
	if (has_year || has_month || has_day)
	{
		CalendarDate restored;
		restored.year = (int)(it["year"] | 0);
		restored.month = (uint8_t)(it["month"] | 0);
		restored.day = (uint8_t)(it["day"] | 0);
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
		Widget::place(m_contentclip, in_layout);
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
	if (m_date.is_valid())
	{
		m_today_button->text(translated_message("calendar.action.today"));
		update_day_combo();

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

// Populate valid day choices for the currently selected month.
void Calendar::update_day_combo()
{
	if (m_date.is_valid())
	{
		m_day_combo->clear();
		uint8_t days = Date::days_in_month(m_date.year, m_date.month);
		for (uint8_t day = 1; day <= days; day++)
		{
			String text;
			text.print("%u", (unsigned int)day);
			ListItem * item = m_day_combo->append(text);
			if (item)
			{
				item->bind(this, &Calendar::on_day_selection);
			}
		}
		m_day_combo->select((int)m_date.day - 1);
	}
}

// Populate editable year suggestions around the selected year.
void Calendar::update_year_combo()
{
	if (m_date.is_valid())
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

// Update the fixed-capacity weekday and date grid.
void Calendar::update_date_grid()
{
	if (m_table && m_date.is_valid())
	{
		m_table->grid_color(grid_color());
		m_table->grid_visible(grid_thickness() > 0);
		m_table->horizontal_thickness(grid_thickness());
		m_table->vertical_thickness(grid_thickness());
		m_table->alternating_row_color1(Color::TRANSPARENT);
		m_table->alternating_row_color2(Color::TRANSPARENT);
		m_table->border_color(grid_color());
		m_table->thickness(grid_thickness());

		for (Dim column = 0; column < 7; column++)
		{
			Button * weekday = dynamic_cast<Button*>(m_table->get_widget(0, column));
			if (weekday)
			{
				weekday->color(weekday_back_color());
				weekday->text_color(weekday_text_color());
				weekday->font_size(weekday_font_size());
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
					cell->font_size(day_font_size());
					bool outside_month = cell_date.year != m_date.year || cell_date.month != m_date.month;
					bool selected = cell_date == m_date;
					cell->color(selected ? day_selected_back_color() :
						(outside_month ? outside_day_back_color() : day_back_color()));
					cell->text_color(selected ? day_selected_text_color() :
						(outside_month ? outside_day_text_color() : day_text_color()));
					cell->border_color(day_border_color());
					cell->thickness(day_border_thickness());
					cell->enabled(true);
				}
				else
				{
					m_cell_dates[cell_index] = CalendarDate();
					cell->text("");
					cell->color(Color::TRANSPARENT);
					cell->text_color(outside_day_text_color());
					cell->border_color(day_border_color());
					cell->thickness(day_border_thickness());
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

// Select a day chosen from the day combo.
void Calendar::on_day_selection(Widget * widget, const ClickEvent & event)
{
	(void)event;
	ListItem * clicked_item = dynamic_cast<ListItem*>(widget);
	int selected_index = -1;
	for (int index = 0; clicked_item && index < (int)m_day_combo->count(); index++)
	{
		if (m_day_combo->at(index) == clicked_item)
		{
			selected_index = index;
		}
	}
	if (selected_index >= 0 && m_date.is_valid())
	{
		CalendarDate updated = m_date;
		updated.day = (uint8_t)(selected_index + 1);
		commit_date(updated, true);
	}
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
		if (commit_date(updated, true))
		{
			update_day_combo();
		}
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
		if (commit_date(updated, true))
		{
			update_day_combo();
		}
	}
}

// Apply day and month combo changes made with the keyboard.
void Calendar::on_day_month_key(Widget * widget, const KeyEvent & event)
{
	if (event.state() == KeyEvent::KEY_UP && m_date.is_valid())
	{
		if (widget == m_day_combo)
		{
			int selected_index = m_day_combo->selected_index();
			if (selected_index >= 0)
			{
				CalendarDate updated = m_date;
				updated.day = (uint8_t)(selected_index + 1);
				commit_date(updated, true);
			}
		}
		else if (widget == m_month_combo)
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
				if (commit_date(updated, true))
				{
					update_day_combo();
				}
			}
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
				update_day_combo();
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
