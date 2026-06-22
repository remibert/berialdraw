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
		label4->text(" &~'\\{([-|_\\^@)]+=}$%%?,.;/:!");

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
	Window window;
		window.position(0,0);
		window.size(480,200);
		window.color(Color::WHITE);

	// Test rich text with different colors
	Label * label1 = new Label(&window);
		label1->text("Hello $<'text-color':4294901760>Red$$ World");
		label1->position(10,10);
		label1->font_size(20);

	// Test rich text with bold font
	Label * label2 = new Label(&window);
		label2->text("Normal $<'font-familly':'Cerial-bold'>Bold$$ Normal");
		label2->position(10,50);
		label2->font_size(20);

	// Test rich text with different font size
	Label * label3 = new Label(&window);
		label3->text("Small $<'font-size':{'width':32,'height':32}>BIG$$ Small");
		label3->position(10,90);
		label3->font_size(16);

	// Test rich text with combined attributes: bold + color + size
	Label * label4 = new Label(&window);
		label4->text("Mix $<'font-familly':'Cerial-bold','font-size':{'width':28,'height':28},'text-color':4278233600>GreenBold$$ end");
		label4->position(10,140);
		label4->font_size(16);

	 UIManager::desktop()->dispatch("$(ui.tests)/out/label3_1.svg");
}

void Label::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,250);
		window.color(Color::WHITE);

	// Test multiline with rich text
	Label * label1 = new Label(&window);
		label1->text("Line1 $<'text-color':4278190335>Blue$$\nLine2 $<'font-familly':'Cerial-bold','text-color':4294901760>RedBold$$\nLine3 Normal");
		label1->position(10,10);
		label1->font_size(20);

	// Test multiple spans on same line
	Label * label2 = new Label(&window);
		label2->text("$<'text-color':4294901760>R$$$<'text-color':4278233600>G$$$<'text-color':4278190335>B$$ RGB");
		label2->position(10,100);
		label2->font_size(24);

	// Test text with no formatting tags (backward compatibility)
	Label * label3 = new Label(&window);
		label3->text("Plain text no formatting");
		label3->position(10,150);
		label3->font_size(20);

	// Test different sizes on same line
	Label * label4 = new Label(&window);
		label4->text("A$<'font-size':{'width':30,'height':30}>B$$$<'font-size':{'width':40,'height':40}>C$$$<'font-size':{'width':20,'height':20}>D$$");
		label4->position(10,190);
		label4->font_size(14);
	 UIManager::desktop()->dispatch("$(ui.tests)/out/label4_1.svg");
}

void Label::test5()
{
	Window window;
	Label * label4 = new Label(&window);
		label4->position(10,140);
		label4->font_size(16);
		label4->text(
			"$<'font-familly':'Cerial','font-size':{'width':50,'height':80},'text-color':0xFF00FF00>"
			"GreenpÂ"
			"$<'font-familly':'Cerial','font-size':{'width':20,'height':20},'text-color':0xFFFF0000>"
			"Redp_$$endÂ");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_1.svg");
	label4->text("Mix$<'font-familly':'Cerial','font-size':{'width':32,'height':32},'text-color':0xFF00FF00>Gr$$end");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_2.svg");
	label4->text("Mix$<'font-familly':'Cerial','font-size':{'width':32,'height':32},'text-color':0xFF00FF00>Gre$$end");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_3.svg");
	label4->text("Mix$<'font-familly':'Cerial','font-size':{'width':32,'height':32},'text-color':0xFF00FF00>Gren$$end");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_4.svg");
}

void Label::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
