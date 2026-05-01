#include "berialdraw_imp.hpp"
using namespace berialdraw;

void WidgetStyle::test1()
{
	WidgetStyle prop;
	String buffer;
	Json json;
	JsonIterator win(json);
	prop.serialize(win);
	json.serialize(buffer,1);

	json.unserialize(buffer.c_str());
}

void WidgetStyle::test2()
{
	uint32_t s;
	s = sizeof(Margin);
	s = sizeof(String);
	s = sizeof(WidgetStyle);
	s = sizeof(BorderStyle);
}

void WidgetStyle::test3()
{
}

void WidgetStyle::test4()
{
}

void WidgetStyle::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
