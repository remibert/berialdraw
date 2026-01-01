#include "berialdraw_imp.hpp"

using namespace berialdraw;

Switch::Switch(Widget * parent):
	Widget("switch", parent, sizeof(Switch))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
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

/** Return the size of content without marges */
Size Switch::content_size()
{
	return m_switch_size;
}


void Switch::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}
	place_in_area(area, in_layout);

	// If absolute place
	if (in_layout == false)
	{
		Area backclip = m_foreclip;
		Margin marg;
		Size size(content_size());
		place_in_layout(backclip, size, marg, EXTEND_NONE, m_foreclip, (Align)m_align);
	}
}

void Switch::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		uint32_t track_color = checked() ? stated_color(m_on_track_color) : stated_color(m_off_track_color);
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		Area area_track(m_foreclip);

		Rect::build_focused_polygon(m_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(track_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT,
			stated_color(m_focus_color),
			m_focused);

		Area area_thumb(m_foreclip);

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
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Switch::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
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

	UIManager::notifier()->play_script(script, "${tests}/out/switch1_%d.svg");
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

	UIManager::notifier()->play_script(script, "${tests}/out/switch2_%d.svg");
	assert(test_switch.m_checked == 4 && test_switch.m_unchecked == 4);
}

void Switch::test3()
{
}

void Switch::test4()
{
}

void Switch::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
