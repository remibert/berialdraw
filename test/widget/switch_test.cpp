#include "berialdraw_imp.hpp"
using namespace berialdraw;

void Switch::test1()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(64,10);
		window.size(200,470);
		window.color(Color::WHITE);

	Grid * grid = new Grid(&window);
	
	int row = 0;
	Switch * on;

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->radius(2);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->switch_size(64,40);
			on->on_track_color(Color::RED);
			on->radius(5);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->switch_size(128,40);
			on->on_track_color(Color::RED);
			on->radius(20);
			on->thumb_padding(3);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->extend(Extend::EXTEND_WIDTH);
			on->size_policy(SizePolicy::SHRINK_HEIGHT);
			on->thumb_color(Color::ORANGE);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->radius(2);
			on->extend(Extend::EXTEND_WIDTH);
			on->size_policy(SizePolicy::SHRINK_HEIGHT);
			on->off_track_color(Color::ROSE);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->switch_size(64,40);
			on->on_track_color(Color::RED);
			on->radius(5);
			on->extend(Extend::EXTEND_WIDTH);
			on->size_policy(SizePolicy::SHRINK_HEIGHT);

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->switch_size(128,40);
			on->on_track_color(Color::RED);
			on->radius(20);
			on->thumb_padding(3);
			on->extend(Extend::EXTEND_WIDTH);
			on->size_policy(SizePolicy::SHRINK_HEIGHT);

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

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/switch1_%d.svg");
}


class TestSwitch
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

void Switch::test2()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(64,10);
		window.size(100,100);
		window.color(Color::WHITE);

	Grid * grid = new Grid(&window);
	
	int row = 0;

	TestSwitch test_switch;

	Switch * on;

		on = new Switch(grid);
			on->cell(row++,0);
			on->id(row);
			on->bind(&test_switch, &TestSwitch::on_check);

	//while(1) UIManager::desktop()->dispatch();

	String script(
	"["
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'touch','x':121,'y': 58,'state':'down'},"
		"{'type':'touch','x':121,'y': 58,'state':'up'},"
		"{'type':'touch','x':121,'y': 58,'state':'down'},"
		"{'type':'touch','x':121,'y': 58,'state':'up'},"
		"{'type':'touch','x':121,'y': 58,'state':'move'},"
	"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/switch2_%d.svg");
	assert(test_switch.m_checked == 4 && test_switch.m_unchecked == 4);
}

void Switch::test3()
{
}

void Switch::test4()
{
	Window window;
		window.position(0,0);
		window.size(400,150);
		window.color(Color::WHITE);

	Column * col = new Column(&window);
	
		// Switch without text
		Switch * switch1 = new Switch(col);
			switch1->text("");
			switch1->margin(10);
		
		// Switch with text
		Switch * switch2 = new Switch(col);
			switch2->text("Enable notifications");
			switch2->margin(10);
		
		// Switch with text and custom styling
		Switch * switch3 = new Switch(col);
			switch3->text("Dark mode");
			switch3->text_color(Color::BLUE);
			switch3->font_size(18);
			switch3->margin(10);
	
	UIManager::desktop()->dispatch("$(ui.tests)/out/switch4.svg");
}

void Switch::test5()
{
	// Test absolute positioning with text
	Window window;
		window.position(0,0);
		window.size(400,200);
		window.color(Color::WHITE);

	// Switch with absolute position and text
	Switch * switch1 = new Switch(&window);
		switch1->position(20, 20);
		switch1->size(350, 50);
		switch1->text("Absolute position with text");
		switch1->text_color(Color::RED);
		switch1->font_size(16);
		switch1->color(Color::LIGHT_GRAY);
		switch1->border_color(Color::BLACK);
		switch1->thickness(2);

	// Switch with absolute position without text
	Switch * switch2 = new Switch(&window);
		switch2->position(20, 80);
		switch2->text("");
		switch2->color(Color::LIGHT_GRAY);
		switch2->border_color(Color::BLACK);
		switch2->thickness(2);

	// Switch with absolute position and different text
	Switch * switch3 = new Switch(&window);
		switch3->position(20, 130);
		switch3->size(350, 50);
		switch3->text("Another absolute switch");
		switch3->text_color(Color::GREEN);
		switch3->font_size(25);
		switch3->color(Color::LIGHT_GRAY);
		switch3->border_color(Color::BLACK);
		switch3->thickness(2);

	//while(1) UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/switch5.svg");
}

void Switch::test6()
{
	Window window;
	Column * column = new Column(&window);
	Switch * switch_;
		switch_ = new Switch(column);
		switch_->text("Switch");
		switch_->font_size(5);

		switch_ = new Switch(column);
		switch_->text("Switch");
		switch_->font_size(25);

		switch_ = new Switch(column);
		switch_->text("Switch");
		switch_->font_size(55);

		switch_ = new Switch(column);
		switch_->text("Switch");
		switch_->font_size(255);

	UIManager::desktop()->dispatch("$(ui.tests)/out/switch6.svg");
}

void Switch::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
