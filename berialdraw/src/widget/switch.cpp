#include "berialdraw_imp.hpp"

using namespace berialdraw;

Switch::Switch(Widget * parent):
	Widget("switch", parent, sizeof(Switch))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (SwitchStyle*)this);
	bind(this, &Switch::on_key);
	bind(this, &Switch::on_click);
}

Switch::~Switch()
{
}

/** Copy all styles of the switchar */
void Switch::copy(const Switch & switch_)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&switch_);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&switch_);
	*((TextStyle*)this) = *(TextStyle*)(&switch_);
	*((BorderStyle*)this) = *(BorderStyle*)(&switch_);
	*((SwitchStyle*)this) = *(SwitchStyle*)(&switch_);
}

/** Copy all styles of the switchar */
void Switch::copy(const Switch * switch_)
{
	if(switch_)
	{
		copy(*switch_);
	}
}

/** Return the size of content without margins */
Size Switch::content_size()
{
	Size result = m_switch_size;

	if(m_text_modified || m_font_modified)
	{
		Area area;
		select_font();
		if (m_font.get())
		{
			m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		}
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}
	
	// Add text size to the right if not empty
	if (m_text.size() > 0)
	{
		result.increase_(padding().left_() + (10<<6) + m_text_size.width_() + padding().right_(), 0);
		if (m_text_size.height_() > m_switch_size.height_())
		{
			result.height_(m_text_size.height_());
		}
	}
	return result;
}

/** Place all widget in area */
void Switch::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place the switch
	m_switch_foreclip = m_foreclip;

	// If text not empty
	if (m_text.size() > 0)
	{
		Margin marg;

		// Place switch text
		m_text_backclip = m_foreclip;
		marg.left_(m_switch_size.width_() + padding().left_());
		place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
		m_switch_foreclip = m_text_foreclip;
		m_switch_foreclip.size(m_switch_size);
		Coord move_y = (m_switch_size.height_() > m_text_size.height_() ? 0-((m_switch_size.height_() - m_text_size.height_())>>1) : ((m_text_size.height_()-m_switch_size.height_())>>1));
		m_switch_foreclip.position().move_(0-(m_switch_size.width_() + padding().left_()), move_y);
		m_switch_foreclip.position().nearest_pixel();
	}
}

/** Paint on screen memory the content of this widget */
void Switch::paint_switch(Region & region)
{
	uint32_t track_color = checked() ? stated_color(m_on_track_color) : stated_color(m_off_track_color);
	Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

	UIManager::renderer()->region(region);

	// Create an area for just the switch (not including text)
	Area area_track(m_switch_foreclip);

	Rect::build_focused_polygon(area_track, 
		*(CommonStyle*)this,
		*(BorderStyle*)this,
		stated_color(track_color), 
		stated_color(m_border_color),
		Color::TRANSPARENT,
		stated_color(m_focus_color),
		m_focused);

	Area area_thumb(area_track);

	area_thumb.size().decrease_(m_thumb_padding << 1, m_thumb_padding << 1);
	area_thumb.position().move_(m_thumb_padding, m_thumb_padding);

	// Change position of thumb
	if (m_checked)
	{
		if (m_extend & Extend::EXTEND_WIDTH)
		{
			area_thumb.position().move(m_foreclip.width() - m_switch_size.height(), 0);
		}
		else
		{
			area_thumb.position().move(m_switch_size.width() - m_switch_size.height(), 0);
		}
	}
	area_thumb.size().width(area_thumb.size().height());
	Rect::build_polygon(area_thumb, substract(m_radius, m_thumb_padding), 0, 0, ALL_BORDERS, stated_color(m_thumb_color), 0);

}


void Switch::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		paint_switch(region);
		
		// Paint text to the right
		if(m_text.size() > 0)
		{
			region.intersect(m_text_backclip);
			select_font();
			UIManager::renderer()->region(region);
			Point shift;
			m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, stated_color(m_text_color), 0, 0, true);
		}
	}
}

/** Get the widget hovered */
Widget * Switch::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void Switch::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	SwitchStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Switch::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	SwitchStyle::unserialize(it);
}


/** Call back on key */
void Switch::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_BACKSPACE || evt.key() == (wchar_t)ReservedKey::KEY_DELETE)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = (m_checked == 0 ? 1 : 0);
				UIManager::notifier()->check(m_checked, this);
			}
			else if (evt.key() == (wchar_t)ReservedKey::KEY_RIGHT)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = 1;
				UIManager::notifier()->check(m_checked, this);
			}
			else if (evt.key() == (wchar_t)ReservedKey::KEY_LEFT)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = 0;
				UIManager::notifier()->check(m_checked, this);
			}
			else if (evt.key() == (wchar_t)ReservedKey::KEY_SPACE)
			{
				UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				m_checked = (m_checked == 0 ? 1 : 0);
				UIManager::notifier()->check(m_checked, this);
			}
		}
	}
}

/** Call back on click */
void Switch::on_click(Widget * widget, const ClickEvent & evt)
{
	m_checked = (m_checked == 0 ? 1 : 0);
	UIManager::notifier()->check(m_checked, this);
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
}


#ifdef _DEBUG
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
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
