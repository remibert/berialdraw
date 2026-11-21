#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Check whether the date is valid in the supported Gregorian range.
bool CalendarDate::is_valid() const
{
	return year >= Date::MIN_YEAR && year <= Date::MAX_YEAR &&
		month >= 1 && month <= 12 &&
		day >= 1 && day <= Date::days_in_month(year, month);
}

// Compare two civil dates.
bool CalendarDate::operator==(const CalendarDate & other) const
{
	return year == other.year && month == other.month && day == other.day;
}

// Compare two civil dates for inequality.
bool CalendarDate::operator!=(const CalendarDate & other) const
{
	return !(*this == other);
}

// Get the local civil date from the operating system.
bool Date::today(CalendarDate & result)
{
	bool success = false;
	int year = 0;
	int month = 0;
	int day = 0;

	if (bd_get_local_date(&year, &month, &day))
	{
		if (month >= 1 && month <= 12 && day >= 1 && day <= 31)
		{
			CalendarDate current;
			current.year = year;
			current.month = (uint8_t)month;
			current.day = (uint8_t)day;
			if (current.is_valid())
			{
				result = current;
				success = true;
			}
		}
	}
	return success;
}

// Get the number of days in a Gregorian month.
uint8_t Date::days_in_month(int year, uint8_t month)
{
	static const uint8_t month_days[] =
	{
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};

	uint8_t result = 0;
	if (year >= MIN_YEAR && year <= MAX_YEAR && month >= 1 && month <= 12)
	{
		result = month_days[month - 1];
		bool leap_year = (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
		if (month == 2 && leap_year)
		{
			result = 29;
		}
	}
	return result;
}

// Get the weekday with Monday as zero.
uint8_t Date::weekday_monday(const CalendarDate & date)
{
	uint8_t result = 7;
	if (date.is_valid())
	{
		int64_t previous_year = date.year - 1;
		int64_t days = 365 * previous_year + previous_year / 4 -
			previous_year / 100 + previous_year / 400;
		for (uint8_t month = 1; month < date.month; month++)
		{
			days += days_in_month(date.year, month);
		}
		days += date.day - 1;
		result = (uint8_t)(days % 7);
	}
	return result;
}
