#pragma once
namespace berialdraw
{
	class Button;
	class Column;
	class Combo;
	class Label;
	class Row;
	class TableView;

	/** Monthly date selector with localized controls and a clickable week grid. */
	class Calendar : public Widget, public CalendarStyle
	{
	public:
		/** Create a calendar initialized to the current local date.
		@param parent Parent widget */
		Calendar(Widget * parent);

		/** Create a calendar initialized to a specified date.
		@param parent Parent widget
		@param date Initial date; must be valid */
		Calendar(Widget * parent, const CalendarDate & date);

		/** Destroy the calendar and its child controls. */
		virtual ~Calendar();

		/** Return the selected date.
		@return Selected date, or an invalid date if initialization had no usable date */
		const CalendarDate & date() const;

		/** Select a date.
		@param value Date to select
		@return True if the date is valid and was accepted */
		bool date(const CalendarDate & value);

		/** Return the selected year. */
		int year() const;

		/** Select a year, clamping the day to the last valid day of the month.
		@param value Gregorian year in the range 1 through 9999
		@return True if the year was accepted */
		bool year(int value);

		/** Return the selected month, from 1 through 12. */
		uint8_t month() const;

		/** Select a month, clamping the day to the last valid day of the month.
		@param value Month in the range 1 through 12
		@return True if the month was accepted */
		bool month(uint8_t value);

		/** Return the selected day of the month. */
		uint8_t day() const;

		/** Select a valid day in the current month.
		@param value Day of the month
		@return True if the day was accepted */
		bool day(uint8_t value);

		/** Move the selected date to the previous month, clamping its day if needed. */
		void previous_month();

		/** Move the selected date to the next month, clamping its day if needed. */
		void next_month();

		/** Select the current local date.
		If the system date is unavailable, the current selection remains unchanged. */
		void today();

		/** Serialize the calendar styles and selected date.
		@param it JSON iterator that receives the calendar data */
		void serialize(JsonIterator & it) override;

		/** Restore calendar styles and the selected date.
		@param it JSON iterator containing the calendar data */
		void unserialize(JsonIterator & it) override;

		/** Return the calendar style cascade mode. */
		StyleCascadeMode style_cascade_mode() const override;

#ifdef _DEBUG
		static void test();
		static void test1();
#endif

	private:
		void initialize();
		void build_controls();
		void update_controls();
		void update_day_combo();
		void update_year_combo();
		void update_table_grid_style();
		void update_weekday_style(Button * weekday);
		void update_day_style(Button * day, bool outside_month, bool selected);
		void update_date_grid();
		bool commit_date(const CalendarDate & value, bool notify);
		bool parse_year(const String & value, int & year) const;
		void on_previous_month(Widget * widget, const ClickEvent & event);
		void on_next_month(Widget * widget, const ClickEvent & event);
		void on_today(Widget * widget, const ClickEvent & event);
		void on_day_selection(Widget * widget, const ClickEvent & event);
		void on_month_selection(Widget * widget, const ClickEvent & event);
		void on_year_selection(Widget * widget, const ClickEvent & event);
		void on_day_month_key(Widget * widget, const KeyEvent & event);
		void on_year_key(Widget * widget, const KeyEvent & event);
		void on_date_cell_click(Widget * widget, const ClickEvent & event);
		void on_date_cell_key(Widget * widget, const KeyEvent & event);
		void place(const Area & area, bool in_layout) override;

		CalendarDate m_date;
		Column * m_content = nullptr;
		Button * m_today_button = nullptr;
		Button * m_previous_button = nullptr;
		Button * m_next_button = nullptr;
		Combo * m_day_combo = nullptr;
		Combo * m_month_combo = nullptr;
		Combo * m_year_combo = nullptr;
		TableView * m_table = nullptr;
		Button * m_day_buttons[49] = {};
		CalendarDate m_cell_dates[49];
		bool m_cell_date_valid[49] = {};
		Dim m_visible_weeks = 0;
	};
}
