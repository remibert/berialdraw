#include "berialdraw_imp.hpp"
using namespace berialdraw;

#include <math.h>
#include <stdio.h>
void tools_test1()
{
	uint32_t i;
	uint32_t res;
	uint32_t shift = 6;
	double r1, r2;

	for (i = 0; i < 0x01000000; i += 1)
	{
		res = (uint32_t)berialdraw::sqr((uint64_t)(i << shift));

		r1 = ((float)res) / (1 << (shift / 2));
		r2 = sqrt((int32_t)i);
		if (abs(int32_t(r1 - r2)) >= 1)
		{
			bd_printf("%d %.4lf %.4lf %d\n", i, r1, r2, abs(int32_t(r1 - r2)));
		}
	}
}

// Test language catalog selection and fallback.
static void test_language_catalogs()
{
	Language * language = UIManager::language();
	assert(language != nullptr);
	assert(language->locale() == "en-US");
	assert(language->translate("calendar.action.today") == "Today");

	assert(language->select("fr-FR"));
	assert(language->translate("calendar.action.today") == "Aujourd’hui");
	assert(language->translate("unknown.message") == "unknown.message");


	assert(language->select("de-DE"));
	assert(language->select("../en-US") == false);
	assert(language->locale() == "de-DE");
	assert(language->select("en-US"));
}

// Test Gregorian date validation and calendar date adjustment.
static void test_calendar_dates()
{
	assert(Date::days_in_month(2024, 2) == 29);
	assert(Date::days_in_month(2023, 2) == 28);
	assert((Date::weekday_monday(CalendarDate{2026, 11, 1}) == 6));
	assert((CalendarDate{2023, 2, 29}.is_valid() == false));

	Calendar calendar(nullptr, CalendarDate{2024, 1, 31});
	assert(calendar.month(2));
	assert((calendar.date() == CalendarDate{2024, 2, 29}));
	assert(calendar.year(2023));
	assert((calendar.date() == CalendarDate{2023, 2, 28}));
	assert(calendar.date(CalendarDate{2023, 2, 29}) == false);
	calendar.next_month();
	assert((calendar.date() == CalendarDate{2023, 3, 28}));
	calendar.previous_month();
	assert((calendar.date() == CalendarDate{2023, 2, 28}));
	UIManager::notifier()->dispatch();
}

void tools_test2()
{
	FT_Matrix m = berialdraw::vector_matrix(0);

	assert(m.xx == 65536 && m.yy == 65536 && m.xy == 0 && m.yx == 0);
	m = berialdraw::vector_matrix(FT_ANGLE_PI2);
	assert(m.xx == 0 && m.yy == 0 && m.xy == 65536 && m.yx == -65536);
	m = berialdraw::vector_matrix(FT_ANGLE_PI);
	assert(m.xx == -65536 && m.yy == -65536 && m.xy == 0 && m.yx == 0);
	m = berialdraw::vector_matrix(FT_ANGLE_PI + FT_ANGLE_PI2);
	assert(m.xx == 0 && m.yy == 0 && m.xy == -65536 && m.yx == 65536);

	test_language_catalogs();
	test_calendar_dates();
}
