#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Checkbox::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(64,10);
		window.size(200,470);
		window.color(Color::WHITE);

	Grid * grid = new Grid(&window);
	
	int row = 0;
	Checkbox * checkbox;

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->radius(2);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->checkbox_size(24,24);
			checkbox->radius(2);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->checkbox_size(32,32);
			checkbox->radius(4);
			checkbox->thickness(2);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->extend(Extend::EXTEND_WIDTH);
			checkbox->size_policy(SizePolicy::SHRINK_HEIGHT);
			checkbox->check_color(Color::BLUE);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->radius(2);
			checkbox->extend(Extend::EXTEND_WIDTH);
			checkbox->size_policy(SizePolicy::SHRINK_HEIGHT);
			checkbox->checkbox_size(16,16);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->checkbox_size(24,24);
			checkbox->radius(2);
			checkbox->extend(Extend::EXTEND_WIDTH);
			checkbox->size_policy(SizePolicy::SHRINK_HEIGHT);

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->checkbox_size(32,32);
			checkbox->radius(4);
			checkbox->thickness(2);
			checkbox->extend(Extend::EXTEND_WIDTH);
			checkbox->size_policy(SizePolicy::SHRINK_HEIGHT);

	Label * label = new Label(grid);
			label->text(" ");
			label->cell(row++,0);

	//while(1) UIManager::desktop()->dispatch();
	
	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':169,'y': 44,'state':'down'},"
		"{'type':'touch','x':169,'y': 44,'state':'up'},"
		"{'type':'touch','x':165,'y':115,'state':'down'},"
		"{'type':'touch','x':165,'y':115,'state':'up'},"
		"{'type':'touch','x':168,'y':186,'state':'down'},"
		"{'type':'touch','x':168,'y':186,'state':'up'},"
		"{'type':'touch','x':168,'y':260,'state':'down'},"
		"{'type':'touch','x':168,'y':260,'state':'up'},"
		"{'type':'touch','x':165,'y':301,'state':'down'},"
		"{'type':'touch','x':165,'y':301,'state':'up'},"
		"{'type':'touch','x':165,'y':325,'state':'down'},"
		"{'type':'touch','x':165,'y':325,'state':'up'},"
		"{'type':'touch','x':167,'y':356,'state':'down'},"
		"{'type':'touch','x':167,'y':356,'state':'up'},"
		"{'type':'touch','x':166,'y':391,'state':'down'},"
		"{'type':'touch','x':166,'y':391,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':166,'y':391,'state':'down'},"
		"{'type':'touch','x':166,'y':391,'state':'up'},"
		"{'type':'touch','x':150,'y':355,'state':'down'},"
		"{'type':'touch','x':150,'y':355,'state':'up'},"
		"{'type':'touch','x':149,'y':318,'state':'down'},"
		"{'type':'touch','x':149,'y':318,'state':'up'},"
		"{'type':'touch','x':146,'y':295,'state':'down'},"
		"{'type':'touch','x':146,'y':295,'state':'up'},"
		"{'type':'touch','x':144,'y':252,'state':'down'},"
		"{'type':'touch','x':144,'y':252,'state':'up'},"
		"{'type':'touch','x':145,'y':176,'state':'down'},"
		"{'type':'touch','x':145,'y':176,'state':'up'},"
		"{'type':'touch','x':159,'y':119,'state':'down'},"
		"{'type':'touch','x':159,'y':119,'state':'up'},"
		"{'type':'touch','x':161,'y': 48,'state':'down'},"
		"{'type':'touch','x':161,'y': 48,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':'shift','character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':'shift','character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/checkbox1_%d.svg");
}


class TestCheckbox
{
public:
	
	void on_check(Widget * widget, const CheckEvent & evt)
	{
		if (evt.checked())
		{
			m_checked++;
		}
		else
		{
			m_unchecked ++;
		}
	}

	int m_checked = 0;
	int m_unchecked = 0;
};

void Checkbox::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(64,10);
		window.size(100,100);

	Grid * grid = new Grid(&window);
	
	int row = 0;

	TestCheckbox test_checkbox;

	Checkbox * checkbox;

		checkbox = new Checkbox(grid);
			checkbox->cell(row++,0);
			checkbox->id(row);
			checkbox->bind(&test_checkbox, &TestCheckbox::on_check);

	String script(
	"["
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':32,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':32,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':121,'y': 58,'state':'down'},"
		"{'type':'touch','x':121,'y': 58,'state':'up'},"
		"{'type':'touch','x':121,'y': 58,'state':'down'},"
		"{'type':'touch','x':121,'y': 58,'state':'up'},"
		"{'type':'touch','x':121,'y': 58,'state':'move'},"
	"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/checkbox2_%d.svg");
	assert(test_checkbox.m_checked == 4 && test_checkbox.m_unchecked == 3);
}

void Checkbox::test3()
{
	Window window;
	Column * column = new Column(&window);
	Checkbox * checkbox;
		checkbox = new Checkbox(column);
		checkbox->text("Checkbox");
		checkbox->font_size(5);

		checkbox = new Checkbox(column);
		checkbox->text("Checkbox");
		checkbox->font_size(25);

		checkbox = new Checkbox(column);
		checkbox->text("Checkbox");
		checkbox->font_size(55);

		checkbox = new Checkbox(column);
		checkbox->text("Checkbox");
		checkbox->font_size(255);

	UIManager::desktop()->dispatch("$(ui.tests)/out/checkbox3.svg");
}

void Checkbox::test4()
{
	Window window;
		window.position(0,0);
		window.size(400,150);
		window.color(Color::WHITE);

	Column * col = new Column(&window);
	
		// Checkbox without text
		Checkbox * checkbox1 = new Checkbox(col);
			checkbox1->text("");
			checkbox1->margin(10);
		
		// Checkbox with text
		Checkbox * checkbox2 = new Checkbox(col);
			checkbox2->text("Accept terms and conditions");
			checkbox2->margin(10);
		
		// Checkbox with text and custom styling
		Checkbox * checkbox3 = new Checkbox(col);
			checkbox3->text("Subscribe to newsletter");
			checkbox3->text_color(Color::RED);
			checkbox3->font_size(18);
			checkbox3->margin(10);
	
	UIManager::desktop()->dispatch("$(ui.tests)/out/checkbox4.svg");
}

void Checkbox::test5()
{
	// Test absolute positioning with text
	Window window;
		window.color(Color::WHITE);

	Pane * pane = new Pane(&window);
		pane->size(350,50);
		pane->position(100,50);
		pane->color(Color::LIGHT_GRAY);

	// Checkbox with absolute position and text
	Checkbox * checkbox1 = new Checkbox(&window);
		checkbox1->position(100, 50);
		checkbox1->size(350, 50);
		checkbox1->text("Absolute position with text");
		checkbox1->text_color(Color::BLUE);
		checkbox1->font_size(16);
		checkbox1->color(Color::LIGHT_BLUE);
		checkbox1->border_color(Color::BLUE);
		checkbox1->thickness(2);

	// Checkbox with absolute position without text
	Checkbox * checkbox2 = new Checkbox(&window);
		checkbox2->position(20, 80);
		checkbox2->text("");
		checkbox2->color(Color::LIGHT_GREEN);
		checkbox2->border_color(Color::GREEN);
		checkbox2->thickness(2);

	// Checkbox with absolute position and different text
	Checkbox * checkbox3 = new Checkbox(&window);
		checkbox3->position(20, 130);
		checkbox3->size(350, 50);
		checkbox3->text("Another absolute checkbox");
		checkbox3->text_color(Color::RED);
		checkbox3->font_size(14);
		checkbox3->color(Color::LIGHT_RED);
		checkbox3->border_color(Color::RED);
		checkbox3->thickness(2);

	//while(1) UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/checkbox5.svg");
}

void Checkbox::test()
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
