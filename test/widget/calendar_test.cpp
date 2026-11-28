#include "berialdraw_imp.hpp"

using namespace berialdraw;

class CalendarEventReceiver
{
public:
	void on_date_changed(Widget * widget, const CalendarEvent & event)
	{
		(void)widget;
		m_count++;
		m_date = event.date();
	}

	int count() const
	{
		return m_count;
	}

	const CalendarDate & date() const
	{
		return m_date;
	}

private:
	int m_count = 0;
	CalendarDate m_date;
};

#ifdef _DEBUG
// Keep calendar snapshots reproducible; do not derive this fixture from the system clock.
static const CalendarDate CALENDAR_TEST_DATE{2026, 11, 28};

// Test leap years, date validation, clamping, and month/year navigation.
void Calendar::test1()
{
	assert(Date::days_in_month(2024, 2) == 29);
	assert(Date::days_in_month(2023, 2) == 28);
	assert(Date::weekday_monday(CalendarDate{2026, 11, 1}) == 6);

	Window window;
	Calendar * calendar = new Calendar(&window, CALENDAR_TEST_DATE);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	assert(calendar->date(CalendarDate{2023, 2, 29}) == false);
	assert(calendar->date(CalendarDate{2024, 1, 31}));
	assert(calendar->month(2));
	assert((calendar->date() == CalendarDate{2024, 2, 29}));
	assert(calendar->year(2023));
	assert((calendar->date() == CalendarDate{2023, 2, 28}));

	calendar->next_month();
	assert((calendar->date() == CalendarDate{2023, 3, 28}));
	calendar->previous_month();
	assert((calendar->date() == CalendarDate{2023, 2, 28}));
}

// Test calendar controls, child IDs, table layout, and weekday/day styles.
void Calendar::test2()
{
	Window window;
	ScrollView * scroll = new ScrollView(&window);
	Column * layout = new Column(scroll);
	Calendar * calendar = new Calendar(layout, CALENDAR_TEST_DATE);

	UIManager::desktop()->dispatch();
	assert(calendar->m_content == calendar->children());
	assert(calendar->m_previous_button->id() == Calendar::PREVIOUS_BUTTON_ID);
	assert(calendar->m_month_combo->id() == Calendar::MONTH_COMBO_ID);
	assert(calendar->m_year_combo->id() == Calendar::YEAR_COMBO_ID);
	assert(calendar->m_next_button->id() == Calendar::NEXT_BUTTON_ID);
	assert(calendar->m_today_button->id() == Calendar::TODAY_BUTTON_ID);
	assert(calendar->m_table->id() == Calendar::TABLE_ID);
	for (Dim index = 0; index < 49; index++)
	{
		assert(calendar->m_day_buttons[index]->id() == Calendar::DAY_BUTTON_ID_BASE + index);
	}

	TableView * table = calendar->m_table;
	assert(table->column_count() == 7);
	assert(table->row_count() == 7);
	assert(table->align() == Align::ALIGN_TOP_LEFT);
	Button * weekday_headers[7] = {};
	for (Dim column = 0; column < 7; column++)
	{
		weekday_headers[column] = dynamic_cast<Button*>(table->get_widget(0, column));
		assert(weekday_headers[column] != nullptr);
		assert(weekday_headers[column]->border_color() == Color::TRANSPARENT);
		assert(weekday_headers[column]->thickness() == 0);
		assert(weekday_headers[column]->focusable() == false);
		assert(weekday_headers[column]->pressable() == false);
	}
	for (int tab = 0; tab < 64; tab++)
	{
		UIManager::notifier()->key((wchar_t)ReservedKey::KEY_TABULATION, KeyEvent::KEY_DOWN);
		UIManager::notifier()->dispatch();
		for (Dim column = 0; column < 7; column++)
		{
			assert(weekday_headers[column]->focused() == false);
		}
	}
	assert(calendar->m_month_combo->editable() == false);
	//assert(table->get_widget(1, 0)->color() == calendar->outside_day_back_color());
	//assert(table->get_widget(2, 0)->color() == calendar->day_back_color());
	assert(dynamic_cast<TextStyle*>(table->get_widget(0, 0))->font_size() == calendar->weekday_font_size());
	assert(dynamic_cast<TextStyle*>(table->get_widget(2, 0))->font_size() == calendar->day_font_size());
}

// Test month/year combo keyboard navigation and scroll preservation.
void Calendar::test3()
{
	Window window;
	ScrollView * scroll = new ScrollView(&window);
	Column * layout = new Column(scroll);
	Calendar * calendar = new Calendar(layout, CALENDAR_TEST_DATE);
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar3_1.svg");

	window.focus_to(calendar->m_month_combo);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	UIManager::desktop()->dispatch();
	Point scroll_position = scroll->scroll_position();
	assert(calendar->m_month_combo->open());
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar3_2.svg");
	ListItem * month_item = calendar->m_month_combo->at((int)calendar->month() - 1);
	assert(month_item != nullptr);
	const Area & month_area = month_item->foreclip();
	UIManager::notifier()->click(
		month_area.x() + (Coord)(month_area.width() / 2),
		month_area.y() + (Coord)(month_area.height() / 2),
		month_item);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert(scroll->scroll_position().y_q6() == scroll_position.y_q6());

	KeyEvent month_page_down((wchar_t)ReservedKey::KEY_PAGE_DOWN, KeyEvent::KEY_DOWN);
	window.focus_to(calendar->m_year_combo);
	calendar->on_month_key(calendar->m_month_combo, month_page_down);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	window.focus_to(calendar->m_month_combo);
	calendar->on_month_key(calendar->m_month_combo, month_page_down);
	assert((calendar->date() == CalendarDate{2027, 2, 28}));
	KeyEvent month_page_up((wchar_t)ReservedKey::KEY_PAGE_UP, KeyEvent::KEY_DOWN);
	calendar->on_month_key(calendar->m_month_combo, month_page_up);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	KeyEvent year_page_down((wchar_t)ReservedKey::KEY_PAGE_DOWN, KeyEvent::KEY_DOWN);
	calendar->on_year_key(calendar->m_year_combo, year_page_down);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	window.focus_to(calendar->m_year_combo);
	calendar->on_year_key(calendar->m_year_combo, year_page_down);
	assert((calendar->date() == CalendarDate{2036, 11, 28}));
	KeyEvent year_page_up((wchar_t)ReservedKey::KEY_PAGE_UP, KeyEvent::KEY_DOWN);
	calendar->on_year_key(calendar->m_year_combo, year_page_up);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	UIManager::notifier()->dispatch();

}

// Test selecting dates from previous-month and next-month grid cells.
void Calendar::test4()
{
	Window window;
	Calendar * calendar = new Calendar(&window, CALENDAR_TEST_DATE);

	CalendarEventReceiver receiver;
	calendar->bind(&receiver, &CalendarEventReceiver::on_date_changed);
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar4_1.svg");
	TableView * table = calendar->m_table;

	Widget * previous_month_cell = table->get_widget(1, 5);
	assert(previous_month_cell != nullptr);
	UIManager::notifier()->click(0, 0, previous_month_cell);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2026, 10, 31}));
	assert(receiver.count() == 1);
	assert(receiver.date() == calendar->date());

	assert(calendar->date(CalendarDate{2025, 8, 10}));
	UIManager::notifier()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar4_2.svg");
	Widget * next_month_cell = table->get_widget(6, 0);
	assert(next_month_cell != nullptr);
	UIManager::notifier()->click(0, 0, next_month_cell);
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2025, 9, 1}));
	assert(receiver.count() == 3);
	assert(receiver.date() == calendar->date());

	assert(calendar->date(CalendarDate{2026, 6, 10}));
	UIManager::notifier()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar4_3.svg");
	Widget * previous_month_week_cell = table->get_widget(1, 0);
	assert(previous_month_week_cell != nullptr);
	UIManager::notifier()->click(0, 0, previous_month_week_cell);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2026, 5, 25}));

	assert(calendar->date(CalendarDate{2026, 6, 10}));
	UIManager::notifier()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar4_4.svg");
	Widget * next_month_week_cell = table->get_widget(6, 2);
	assert(next_month_week_cell != nullptr);
	UIManager::notifier()->click(0, 0, next_month_week_cell);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2026, 7, 1}));
}

// Test content-size and marged-size contributions from padding, borders, and margin.
void Calendar::test5()
{
	Window window;
		Column * layout = new Column(&window);
			Button * before = new Button(layout);
				before->text("before");
			Calendar * calendar = new Calendar(layout, CALENDAR_TEST_DATE);
			Button * after = new Button(layout);
				after->text("after");

	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar5_1.svg");
	calendar->margin(17);
	calendar->padding(29);
	calendar->thickness(5);
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar5_2.svg");
}

// Test calendar accessors, control callbacks, keyboard navigation, and date-range boundaries.
void Calendar::test6()
{
	Window window;
	Calendar * calendar = new Calendar(&window, CALENDAR_TEST_DATE);
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar6.svg");

	auto cell_index_for_date = [calendar](const CalendarDate & value)
	{
		int result = -1;
		for (Dim index = 0; index < calendar->m_visible_weeks * 7 && result < 0; index++)
		{
			if (calendar->m_cell_date_valid[index] && calendar->m_cell_dates[index] == value)
			{
				result = (int)index;
			}
		}
		return result;
	};
	auto focus_date_cell = [&](const CalendarDate & value)
	{
		int index = cell_index_for_date(value);
		assert(index >= 0);
		window.focus_to(calendar->m_day_buttons[index]);
	};

	assert(calendar->year() == 2026);
	assert(calendar->month() == 11);
	assert(calendar->day() == 28);
	assert(calendar->year(0) == false);
	assert(calendar->year(10000) == false);
	assert(calendar->month(0) == false);
	assert(calendar->month(13) == false);
	assert(calendar->day(0) == false);
	assert(calendar->day(31) == false);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	assert(calendar->day(27));
	assert(calendar->day() == 27);
	assert(calendar->date(CALENDAR_TEST_DATE));

	// Verify month-control selection clamps the day, then exercise year and month combo keys.
	assert(calendar->date(CalendarDate{2024, 1, 31}));
	ListItem * february = calendar->m_month_combo->at(1);
	assert(february != nullptr);
	ClickEvent february_click(Point(), february);
	calendar->on_month_selection(february, february_click);
	assert((calendar->date() == CalendarDate{2024, 2, 29}));

	ListItem * year_2023 = nullptr;
	for (int index = 0; index < (int)calendar->m_year_combo->count() && year_2023 == nullptr; index++)
	{
		ListItem * item = calendar->m_year_combo->at(index);
		if (item && item->text() == String("2023"))
		{
			year_2023 = item;
		}
	}
	assert(year_2023 != nullptr);
	ClickEvent year_click(Point(), year_2023);
	calendar->on_year_selection(year_2023, year_click);
	assert((calendar->date() == CalendarDate{2023, 2, 28}));

	assert(calendar->date(CalendarDate{2023, 2, 28}));
	calendar->m_month_combo->select(2);
	window.focus_to(calendar->m_month_combo);
	KeyEvent month_release((wchar_t)ReservedKey::KEY_DOWN, KeyEvent::KEY_UP);
	calendar->on_month_key(calendar->m_month_combo, month_release);
	assert((calendar->date() == CalendarDate{2023, 3, 28}));

	assert(calendar->date(CalendarDate{2024, 3, 31}));
	calendar->m_month_combo->select(1);
	window.focus_to(calendar->m_month_combo);
	calendar->on_month_key(calendar->m_month_combo, month_release);
	assert((calendar->date() == CalendarDate{2024, 2, 29}));

	ListItem * keyboard_year_2023 = nullptr;
	for (int index = 0; index < (int)calendar->m_year_combo->count() && keyboard_year_2023 == nullptr; index++)
	{
		ListItem * item = calendar->m_year_combo->at(index);
		if (item && item->text() == String("2023"))
		{
			keyboard_year_2023 = item;
		}
	}
	assert(keyboard_year_2023 != nullptr);
	int keyboard_year_2023_index = -1;
	for (int index = 0; index < (int)calendar->m_year_combo->count() && keyboard_year_2023_index < 0; index++)
	{
		if (calendar->m_year_combo->at(index) == keyboard_year_2023)
		{
			keyboard_year_2023_index = index;
		}
	}
	calendar->m_year_combo->select(keyboard_year_2023_index);
	window.focus_to(calendar->m_year_combo);
	KeyEvent year_release((wchar_t)ReservedKey::KEY_UP, KeyEvent::KEY_UP);
	calendar->on_year_key(calendar->m_year_combo, year_release);
	assert((calendar->date() == CalendarDate{2023, 2, 28}));

	calendar->m_year_combo->text("2020");
	KeyEvent year_enter((wchar_t)ReservedKey::KEY_ENTER, KeyEvent::KEY_DOWN);
	calendar->on_year_key(calendar->m_year_combo, year_enter);
	assert((calendar->date() == CalendarDate{2020, 2, 28}));
	calendar->m_year_combo->text("10000x");
	calendar->on_year_key(calendar->m_year_combo, year_enter);
	assert((calendar->date() == CalendarDate{2020, 2, 28}));

	assert(calendar->date(CalendarDate{2026, 1, 1}));
	window.focus_to(calendar->m_month_combo);
	KeyEvent month_page_up((wchar_t)ReservedKey::KEY_PAGE_UP, KeyEvent::KEY_DOWN);
	calendar->on_month_key(calendar->m_month_combo, month_page_up);
	assert((calendar->date() == CalendarDate{2025, 10, 1}));
	KeyEvent month_page_down((wchar_t)ReservedKey::KEY_PAGE_DOWN, KeyEvent::KEY_DOWN);
	calendar->on_month_key(calendar->m_month_combo, month_page_down);
	assert((calendar->date() == CalendarDate{2026, 1, 1}));

	assert(calendar->date(CalendarDate{1, 1, 1}));
	window.focus_to(calendar->m_year_combo);
	KeyEvent year_page_up((wchar_t)ReservedKey::KEY_PAGE_UP, KeyEvent::KEY_DOWN);
	calendar->on_year_key(calendar->m_year_combo, year_page_up);
	assert((calendar->date() == CalendarDate{1, 1, 1}));
	KeyEvent year_page_down((wchar_t)ReservedKey::KEY_PAGE_DOWN, KeyEvent::KEY_DOWN);
	calendar->on_year_key(calendar->m_year_combo, year_page_down);
	assert((calendar->date() == CalendarDate{11, 1, 1}));
	assert(calendar->date(CalendarDate{9999, 12, 31}));
	calendar->on_year_key(calendar->m_year_combo, year_page_down);
	assert((calendar->date() == CalendarDate{9999, 12, 31}));

	// Exercise explicit today and previous/next button callbacks without snapshotting the local date.
	calendar->today();
	assert(calendar->date().is_valid());
	ClickEvent today_click(Point(), calendar->m_today_button);
	calendar->on_today(calendar->m_today_button, today_click);
	assert(calendar->date().is_valid());
	assert(calendar->date(CalendarDate{2024, 1, 31}));
	ClickEvent next_click(Point(), calendar->m_next_button);
	calendar->on_next_month(calendar->m_next_button, next_click);
	assert((calendar->date() == CalendarDate{2024, 2, 29}));
	ClickEvent previous_click(Point(), calendar->m_previous_button);
	calendar->on_previous_month(calendar->m_previous_button, previous_click);
	assert((calendar->date() == CalendarDate{2024, 1, 29}));

	// Verify selecting a date cell and all date-grid keyboard navigation directions.
	assert(calendar->date(CALENDAR_TEST_DATE));
	int next_day_index = cell_index_for_date(CalendarDate{2026, 11, 29});
	assert(next_day_index >= 0);
	ClickEvent date_click(Point(), calendar->m_day_buttons[next_day_index]);
	calendar->on_date_cell_click(calendar->m_day_buttons[next_day_index], date_click);
	assert((calendar->date() == CalendarDate{2026, 11, 29}));
	ClickEvent ignored_date_click(Point(), calendar);
	calendar->on_date_cell_click(calendar, ignored_date_click);
	assert((calendar->date() == CalendarDate{2026, 11, 29}));

	calendar->date(CALENDAR_TEST_DATE);
	KeyEvent right((wchar_t)ReservedKey::KEY_RIGHT, KeyEvent::KEY_DOWN);
	focus_date_cell(CALENDAR_TEST_DATE);
	calendar->on_date_cell_key(calendar, right);
	assert((calendar->date() == CalendarDate{2026, 11, 29}));
	KeyEvent left((wchar_t)ReservedKey::KEY_LEFT, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, left);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	KeyEvent down((wchar_t)ReservedKey::KEY_DOWN, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, down);
	assert((calendar->date() == CalendarDate{2026, 12, 5}));
	KeyEvent up((wchar_t)ReservedKey::KEY_UP, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, up);
	assert((calendar->date() == CALENDAR_TEST_DATE));
	KeyEvent page_down((wchar_t)ReservedKey::KEY_PAGE_DOWN, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, page_down);
	assert((calendar->date() == CalendarDate{2026, 12, 28}));
	KeyEvent page_up((wchar_t)ReservedKey::KEY_PAGE_UP, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, page_up);
	assert((calendar->date() == CALENDAR_TEST_DATE));

	// Verify month keyboard navigation clamps the day in a leap year.
	assert(calendar->date(CalendarDate{2024, 3, 31}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, page_up);
	assert((calendar->date() == CalendarDate{2024, 2, 29}));
	assert(calendar->date(CALENDAR_TEST_DATE));

	KeyEvent home((wchar_t)ReservedKey::KEY_HOME, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, home);
	assert((calendar->date() == CalendarDate{2026, 11, 1}));
	KeyEvent end((wchar_t)ReservedKey::KEY_END, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, end);
	assert((calendar->date() == CalendarDate{2026, 11, 30}));
	KeyEvent ignored_key((wchar_t)ReservedKey::KEY_TABULATION, KeyEvent::KEY_DOWN);
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, ignored_key);
	assert((calendar->date() == CalendarDate{2026, 11, 30}));

	// Check valid year rollover and reject navigation beyond the supported date range.
	assert(calendar->date(CalendarDate{2026, 1, 1}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, left);
	assert((calendar->date() == CalendarDate{2025, 12, 31}));
	assert(calendar->date(CalendarDate{2026, 12, 31}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, right);
	assert((calendar->date() == CalendarDate{2027, 1, 1}));
	assert(calendar->date(CalendarDate{1, 1, 1}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, left);
	assert((calendar->date() == CalendarDate{1, 1, 1}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, page_up);
	assert((calendar->date() == CalendarDate{1, 1, 1}));
	assert(calendar->date(CalendarDate{9999, 12, 31}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, right);
	assert((calendar->date() == CalendarDate{9999, 12, 31}));
	focus_date_cell(calendar->date());
	calendar->on_date_cell_key(calendar, page_down);
	assert((calendar->date() == CalendarDate{9999, 12, 31}));

	UIManager::notifier()->dispatch();
}

void Calendar::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
