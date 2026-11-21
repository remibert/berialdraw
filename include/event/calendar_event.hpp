#pragma once
namespace berialdraw
{
	/** Event sent when a calendar date is changed. */
	class CalendarEvent : public Event
	{
	public:
		CalendarEvent(const CalendarDate & date, Widget * widget = 0);
		virtual ~CalendarEvent();

		const CalendarDate & date() const;
		Widget * widget() const override;
		static uint32_t type_id();
		void to_string(String & str) const override;

	private:
		CalendarDate m_date;
		Widget * m_widget = 0;
	};
}
