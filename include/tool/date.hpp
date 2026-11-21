#pragma once
namespace berialdraw
{
	/** Civil Gregorian date used by the date and calendar widgets. */
	struct CalendarDate
	{
		int year = 0;
		uint8_t month = 0;
		uint8_t day = 0;

		/** Check whether the date is valid in the supported Gregorian range. */
		bool is_valid() const;

		/** Compare two civil dates. */
		bool operator==(const CalendarDate & other) const;

		/** Compare two civil dates for inequality. */
		bool operator!=(const CalendarDate & other) const;
	};

	/** Date helpers backed by the platform-specific system clock. */
	class Date
	{
	public:
		static constexpr int MIN_YEAR = 1;
		static constexpr int MAX_YEAR = 9999;

		/** Get the local civil date from the operating system.
		@param result Receives the current date on success
		@return True when the operating system provided a valid date */
		static bool today(CalendarDate & result);

		/** Get the number of days in a Gregorian month, or zero for invalid input. */
		static uint8_t days_in_month(int year, uint8_t month);

		/** Get the weekday with Monday as zero, or seven for an invalid date. */
		static uint8_t weekday_monday(const CalendarDate & date);
	};
}
