#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Unitary test
void MaskValidator::test1()
{
	{
		String mask("%d.%d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::ACCEPTED);
	}
	{
		String mask("%z,,");
		MaskValidator valid(mask);
		assert(valid.validate("33,,") == ValidResult::REFUSED);
	}
	{
		String mask("%z,,");
		MaskValidator valid(mask);
		assert(valid.validate("33,,") == ValidResult::REFUSED);
	}
	{
		String mask("%d,,");
		MaskValidator valid(mask);
		assert(valid.validate("33,.") == ValidResult::REFUSED);
	}
	{
		String mask("%[6..3]d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::ACCEPTED);
	}
	{
		String mask("%[3..6]2");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%[3..d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%(3.d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%[3.d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%z,,");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%[d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%%%d");
		MaskValidator valid(mask);
		assert(valid.validate("%3") == ValidResult::ACCEPTED);
	}
	{
		String mask("%d%%");
		MaskValidator valid(mask);
		assert(valid.validate("3%") == ValidResult::ACCEPTED);
	}
	{
		String mask("%[-5..3]d");
		MaskValidator valid(mask);
		assert(valid.validate("5-") == ValidResult::REFUSED);
		assert(valid.validate("-5") == ValidResult::ACCEPTED);
		assert(valid.validate("-6") == ValidResult::REFUSED);
		assert(valid.validate("3") == ValidResult::ACCEPTED);
		assert(valid.validate("4") == ValidResult::REFUSED);
		assert(valid.validate("0") == ValidResult::ACCEPTED);
		assert(valid.validate("-0") == ValidResult::ACCEPTED);
	}
	{
		String mask("%d%");
		MaskValidator valid(mask);
		assert(valid.validate("1.") == ValidResult::REFUSED);
	}
	{
		String mask("%L");
		MaskValidator valid(mask);
		assert(valid.validate("abcz") == ValidResult::REFUSED);
		assert(valid.validate("ABCZ") == ValidResult::ACCEPTED);
		assert(valid.validate("13") == ValidResult::REFUSED);
	}
	{
		String mask("%l");
		MaskValidator valid(mask);
		assert(valid.validate("abcz") == ValidResult::ACCEPTED);
		assert(valid.validate("ABCZ") == ValidResult::REFUSED);
		assert(valid.validate("13") == ValidResult::REFUSED);
	}
	{
		String mask("%2x-%2x");
		MaskValidator valid(mask);
		assert(valid.validate("12.12") == ValidResult::REFUSED);
	}
	{
		String mask("%x");
		MaskValidator valid(mask);
		assert(valid.validate("123abc256") == ValidResult::ACCEPTED);
		assert(valid.validate("123ABC256") == ValidResult::REFUSED);
	}
	{
		String mask("%X");
		MaskValidator valid(mask);
		assert(valid.validate("123abc256") == ValidResult::REFUSED);
		assert(valid.validate("123ABC256") == ValidResult::ACCEPTED);
	}
	{
		String mask("%A");
		MaskValidator valid(mask);
		assert(valid.validate("123abcz256") == ValidResult::REFUSED);
		assert(valid.validate("123ABCZ256") == ValidResult::ACCEPTED);
	}
	{
		String mask("%a");
		MaskValidator valid(mask);
		assert(valid.validate("123abcz256") == ValidResult::ACCEPTED);
		assert(valid.validate("123AbcZ256") == ValidResult::REFUSED);
	}
	{
		String mask("%3[0..255]d.%3[0..255]d.%3[0..255]d.%3[0..255]d");
		MaskValidator valid(mask);
		assert(valid.validate("256") == ValidResult::REFUSED);
	}
	{
		String mask("%02d");
		MaskValidator valid(mask);
		assert(valid.validate("0") == ValidResult::ACCEPTED);
		assert(valid.validate("02") == ValidResult::ACCEPTED);
		assert(valid.validate("1") == ValidResult::ACCEPTED);
		assert(valid.validate("12") == ValidResult::ACCEPTED);
		assert(valid.validate("023") == ValidResult::REFUSED);
	}
	{
		String mask("%3d.%3d.%3d.%3d");
		MaskValidator valid(mask);
		assert(valid.validate(".123.0") == ValidResult::REFUSED);
		assert(valid.validate("a") == ValidResult::REFUSED);
		assert(valid.validate("123.0") == ValidResult::ACCEPTED);
		assert(valid.validate("123.10") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110") == ValidResult::ACCEPTED);
		assert(valid.validate("123.1101") == ValidResult::REFUSED);
		assert(valid.validate("123.011") == ValidResult::REFUSED);

		assert(valid.validate("123.110.") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.0") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.01") == ValidResult::REFUSED);
		assert(valid.validate("123.110.10") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.1012") == ValidResult::REFUSED);

		assert(valid.validate("123.110.101.1") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101.12") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101.123") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101.1123") == ValidResult::REFUSED);
		assert(valid.validate("123.110.101.112.") == ValidResult::REFUSED);
		assert(valid.validate("123.110.1012") == ValidResult::REFUSED);

		assert(valid.validate("123.110.0") == ValidResult::ACCEPTED);
		assert(valid.validate("1") == ValidResult::ACCEPTED);
		assert(valid.validate("01") == ValidResult::REFUSED);
		assert(valid.validate("12") == ValidResult::ACCEPTED);
		assert(valid.validate("123") == ValidResult::ACCEPTED);
		assert(valid.validate("1234") == ValidResult::REFUSED);
		assert(valid.validate("123.") == ValidResult::ACCEPTED);
		
		assert(valid.validate("123.10") == ValidResult::ACCEPTED);
		assert(valid.validate("123.01") == ValidResult::REFUSED);
	}
	{
		String mask("%2d");
		MaskValidator valid(mask);
		assert(valid.validate("") == ValidResult::ACCEPTED);
		assert(valid.validate("1") == ValidResult::ACCEPTED);
		assert(valid.validate("01") == ValidResult::REFUSED);
		assert(valid.validate("12") == ValidResult::ACCEPTED);
		assert(valid.validate("123") == ValidResult::REFUSED);
	}
	{
		String mask("%d");
		MaskValidator valid(mask);
		assert(valid.validate("-01") == ValidResult::REFUSED);
		assert(valid.validate("0") == ValidResult::ACCEPTED);
		assert(valid.validate("01") == ValidResult::REFUSED);
		assert(valid.validate("012") == ValidResult::REFUSED);
		assert(valid.validate("1020") == ValidResult::ACCEPTED);
	}
}

// Unitary test
void MaskValidator::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test1();
	}
}
