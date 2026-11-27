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
void Calendar::test1()
{
	assert(Date::days_in_month(2024, 2) == 29);
	assert(Date::days_in_month(2023, 2) == 28);
	assert(Date::weekday_monday(CalendarDate{2026, 11, 1}) == 6);

	Window window;
	// window.size(480, 480);
	Calendar * calendar = new Calendar(&window, CalendarDate{2026, 11, 19});
	calendar->position(20, 20);
	calendar->size(420, 420);
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
	UIManager::desktop()->dispatch("$(ui.tests)/out/calendar1.svg");
	UIManager::desktop()->mainloop();
	CalendarEventReceiver receiver;
	calendar->bind(&receiver, &CalendarEventReceiver::on_date_changed);
	UIManager::desktop()->dispatch();

	TableView * table = nullptr;
	Widget * content = calendar->children();
	Widget * child = content ? content->children() : nullptr;
	while (child && table == nullptr)
	{
		table = dynamic_cast<TableView*>(child);
		child = child->next();
	}
	assert(table != nullptr);
	assert(table->column_count() == 7);
	assert(table->row_count() == 7);
	assert(table->get_widget(0, 0)->color() == calendar->weekday_back_color());
	Button * weekday_header = dynamic_cast<Button*>(table->get_widget(0, 0));
	assert(weekday_header != nullptr);
	assert(weekday_header->border_color() == Color::TRANSPARENT);
	assert(weekday_header->thickness() == 0);
	assert(weekday_header->focusable() == false);
	assert(weekday_header->pressable() == false);
	assert(calendar->m_month_combo->editable() == false);
	assert(table->get_widget(1, 0)->color() == calendar->outside_day_back_color());
	assert(table->get_widget(2, 0)->color() == calendar->day_back_color());
	assert(dynamic_cast<TextStyle*>(table->get_widget(0, 0))->font_size() == calendar->weekday_font_size());
	assert(dynamic_cast<TextStyle*>(table->get_widget(2, 0))->font_size() == calendar->day_font_size());

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
	Widget * next_month_cell = table->get_widget(6, 0);
	assert(next_month_cell != nullptr);
	UIManager::notifier()->click(0, 0, next_month_cell);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2025, 9, 1}));
	assert(receiver.date() == calendar->date());

	assert(calendar->date(CalendarDate{2026, 6, 10}));
	UIManager::notifier()->dispatch();
	Widget * previous_month_week_cell = table->get_widget(1, 0);
	assert(previous_month_week_cell != nullptr);
	UIManager::notifier()->click(0, 0, previous_month_week_cell);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2026, 5, 25}));

	assert(calendar->date(CalendarDate{2026, 6, 10}));
	UIManager::notifier()->dispatch();
	Widget * next_month_week_cell = table->get_widget(6, 2);
	assert(next_month_week_cell != nullptr);
	UIManager::notifier()->click(0, 0, next_month_week_cell);
	UIManager::notifier()->dispatch();
	UIManager::notifier()->dispatch();
	assert((calendar->date() == CalendarDate{2026, 7, 1}));

	assert(calendar->date(CalendarDate{2024, 1, 31}));
	UIManager::notifier()->dispatch();
	assert(calendar->month(2));
	assert((calendar->date() == CalendarDate{2024, 2, 29}));
	UIManager::notifier()->dispatch();
	assert(calendar->year(2023));
	assert((calendar->date() == CalendarDate{2023, 2, 28}));
	UIManager::notifier()->dispatch();
	assert(calendar->date(CalendarDate{2023, 2, 29}) == false);

	calendar->next_month();
	assert((calendar->date() == CalendarDate{2023, 3, 28}));
	calendar->previous_month();
	assert((calendar->date() == CalendarDate{2023, 2, 28}));
	UIManager::notifier()->dispatch();

	delete calendar;
	UIManager::desktop()->dispatch();
}

void Calendar::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test1();
	}
}
#endif
