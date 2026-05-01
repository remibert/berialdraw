#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Label::test1()
{
	Dim s = 10;
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE_BLUE);

	Label * label1 = new Label(&window);
		label1->text("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	Label * label2 = new Label(&window);
		label2->text("abcdefghijklmnopqrstuvwxyz");

	Label * label3 = new Label(&window);
		label3->text("0123456789");

	Label * label4 = new Label(&window);
		label4->text(" &~\"\\{([-|_\\^@)]+=}$%%?,.;/:!");

	Label * label5 = new Label(&window);
	int id = 0;
	for(Dim s = 0; s < 100; s+=3)
	{
		label1->position(0,s);
		label1->font_size(s,s);

		label2->position(0,2*s);
		label2->font_size(s,s);

		label3->position(0,3*s);
		label3->font_size(s,s);

		label4->position(0,4*s);
		label4->font_size(s,s);

		label5->text("%d",s);
		label5->position(0,5*s);
		label5->font_size(s,s);
		sleep_us(1000);
		UIManager::desktop()->dispatch();

		if (s % 18 == 0)
		{
			String name;
			name.print("$(ui.tests)/out/label1_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
		else
		{
			UIManager::desktop()->dispatch();
		}
	}
}

void Label::test2()
{
	Window window;
		window.position(13,17);
		window.size(450,319);
		window.color(Color::BROWN,32);

	Grid * grid = new Grid(&window);
		Label * label = new Label(grid);
			label->cell(0,0);
			label->text("Hello\nWorld\nHave a nice day");
			label->extend(Extend::EXTEND_NONE);

		label = new Label(grid);
			label->cell(0,1);
			label->text("Hello\nWorld\nHave a nice day");
			label->text_align(Align::ALIGN_LEFT);
			label->extend(Extend::EXTEND_NONE);

		label = new Label(grid);
			label->cell(1,0);
			label->text("Hello\nWorld\nHave a nice day");
			label->text_align(Align::ALIGN_RIGHT);
			label->extend(Extend::EXTEND_NONE);

		label = new Label(grid);
			label->cell(1,1);
			label->text("Hello\nWorld\nHave a nice day");
			label->text_align(Align::CENTER);
			label->extend(Extend::EXTEND_NONE);

	UIManager::desktop()->dispatch("$(ui.tests)/out/label2_1.svg");
}

void Label::test3()
{
}

void Label::test4()
{
}

void Label::test()
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
