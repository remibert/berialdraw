#include "berialdraw_imp.hpp"
using namespace berialdraw;


void Colors::test1()
{
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xFF000000);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("dark") == true);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xffffffff);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("prout") == false);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xffffffff);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("test_light") == true);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xFF000000);
	assert(UIManager::colors()->color(100) == 100);
}

void Colors::test2()
{
}

void Colors::test3()
{
}

void Colors::test4()
{
}

void Colors::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test1();
		test2();
		test3();
		test4();
	}
}
