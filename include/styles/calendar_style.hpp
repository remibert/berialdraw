#pragma once
namespace berialdraw
{
	/** @brief Styles for the calendar grid and date cells.
	Colors and line thicknesses specific to the calendar grid are loaded from calendar.sty. */
	class CalendarStyle : public Style
	{
	public:
		/** Create a calendar style with built-in fallback values. */
		CalendarStyle();

		/** Serialize the calendar style properties.
		@param it JSON iterator that receives the style properties */
		void serialize(JsonIterator & it) override;

		/** Restore calendar style properties from JSON.
		@param it JSON iterator that contains the style properties */
		void unserialize(JsonIterator & it) override;

		/** Return the property name used to load calendar.sty properties. */
		const char * property_name() const override { return "calendar"; }

		/** Copy calendar style properties from another instance.
		@param other Source style */
		CalendarStyle & operator=(const CalendarStyle & other);

		/** Copy calendar style properties from another instance.
		@param other Source style */
		void set(const CalendarStyle & other);

		/** Create a calendar style for the style manager.
		@return Newly allocated style */
		static Style * create();

		/** Return the weekday header text color. */
		uint32_t weekday_text_color() const { return m_weekday_text_color; }

		/** Return the weekday header font size. */
		const Size & weekday_font_size() const { return m_weekday_font_size; }

		/** Return the weekday header background color. */
		uint32_t weekday_back_color() const { return m_weekday_back_color; }

		/** Return the text color for days outside the displayed month. */
		uint32_t outside_day_text_color() const { return m_outside_day_text_color; }

		/** Return the background color for days outside the displayed month. */
		uint32_t outside_day_back_color() const { return m_outside_day_back_color; }

		/** Return the text color for unselected dates. */
		uint32_t day_text_color() const { return m_day_text_color; }

		/** Return the font size for date cells. */
		const Size & day_font_size() const { return m_day_font_size; }

		/** Return the background color for unselected date cells. */
		uint32_t day_back_color() const { return m_day_back_color; }

		/** Return the text color for the selected date. */
		uint32_t day_selected_text_color() const { return m_day_selected_text_color; }

		/** Return the background color for the selected date. */
		uint32_t day_selected_back_color() const { return m_day_selected_back_color; }

		/** Return the date grid line color. */
		uint32_t grid_color() const { return m_grid_color; }

		/** Return the date grid line thickness in pixels. */
		Dim grid_thickness() const { return m_grid_thickness; }

		/** Return the border color for date cells. */
		uint32_t day_border_color() const { return m_day_border_color; }

		/** Return the date cell border thickness in pixels. */
		Dim day_border_thickness() const { return m_day_border_thickness; }

	protected:
		uint32_t m_weekday_text_color = Color::CALENDAR_WEEKDAY_TEXT_COLOR;
		Size m_weekday_font_size = {20, 22};
		uint32_t m_weekday_back_color = Color::CALENDAR_WEEKDAY_BACK_COLOR;
		uint32_t m_outside_day_text_color = Color::CALENDAR_OUTSIDE_DAY_TEXT_COLOR;
		uint32_t m_outside_day_back_color = Color::CALENDAR_OUTSIDE_DAY_BACK_COLOR;
		uint32_t m_day_text_color = Color::CALENDAR_DAY_TEXT_COLOR;
		Size m_day_font_size = {20, 22};
		uint32_t m_day_back_color = Color::CALENDAR_DAY_BACK_COLOR;
		uint32_t m_day_selected_text_color = Color::CALENDAR_DAY_SELECTED_TEXT_COLOR;
		uint32_t m_day_selected_back_color = Color::CALENDAR_DAY_SELECTED_BACK_COLOR;
		uint32_t m_grid_color = Color::CALENDAR_GRID_COLOR;
		Dim m_grid_thickness = 1;
		uint32_t m_day_border_color = Color::CALENDAR_DAY_BORDER_COLOR;
		Dim m_day_border_thickness = 0;
	};
}
