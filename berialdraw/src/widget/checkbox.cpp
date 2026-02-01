#include "berialdraw_imp.hpp"

using namespace berialdraw;

Checkbox::Checkbox(Widget * parent):
	Widget("checkbox", parent, sizeof(Checkbox))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (CheckboxStyle*)this);
	bind(this, &Checkbox::on_key);
	bind(this, &Checkbox::on_click);
}

Checkbox::~Checkbox()
{
}

/** Copy all styles of the checkbox */
void Checkbox::copy(const Checkbox & checkbox)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&checkbox);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&checkbox);
	*((TextStyle*)this) = *(TextStyle*)(&checkbox);
	*((BorderStyle*)this) = *(BorderStyle*)(&checkbox);
	*((CheckboxStyle*)this) = *(CheckboxStyle*)(&checkbox);
}

/** Copy all styles of the checkbox */
void Checkbox::copy(const Checkbox * checkbox)
{
	if(checkbox)
	{
		copy(*checkbox);
	}
}

/** Return the size of content without margins */
Size Checkbox::content_size()
{
	Size result = m_checkbox_size;

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
	
	// Add text height in the icon content size
	if (m_text.size() > 0)
	{
		result.increase_(padding().left_() + m_text_size.width_() + padding().right_(), 0);
		if (m_text_size.height_() > m_checkbox_size.height_())
		{
			result.height_(m_text_size.height_());
		}
	}
	return result;
}

/** Place all widget in area */
void Checkbox::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place the check box
	m_check_foreclip = m_foreclip;

	// If text not empty
	if (m_text.size() > 0)
	{
		Margin marg;

		// Place switch text
		m_text_backclip = m_foreclip;
		marg.left_(m_checkbox_size.width_() + padding().left_());
		place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
		m_check_foreclip = m_text_foreclip;
		m_check_foreclip.size(m_checkbox_size);
		Coord move_y = (m_checkbox_size.height_() > m_text_size.height_() ? 0-((m_checkbox_size.height_() - m_text_size.height_())>>1) : ((m_text_size.height_()-m_checkbox_size.height_())>>1));
		m_check_foreclip.position().move_(0-(m_checkbox_size.width_() + padding().left_()), move_y);
		m_check_foreclip.position().nearest_pixel();
	}
}

void Checkbox::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		// Create an area for just the checkbox (not including text)
		Area area_box(m_check_foreclip);
		area_box.size().set_(m_checkbox_size.width_(), m_checkbox_size.height_());

		// Draw checkbox box
		Rect::build_focused_polygon(area_box, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color),
			stated_color(m_border_color),
			Color::TRANSPARENT,
			stated_color(m_focus_color),
			m_focused);

		// Draw check mark if checked
		if (m_checked)
		{
			Area area_check(area_box);
			area_check.size().decrease_(m_check_padding << 1, m_check_padding << 1);
			area_check.position().move_(m_check_padding, m_check_padding);

			// Parse and draw the check sketch using VectorScript
			if (m_check_sketch.size() > 0)
			{
				Polygon polygon(0);
				VectorsScript script(m_check_sketch.c_str(), polygon);
				
				if (script.parse() == VectorsScript::SUCCESS)
				{
					Coord resolution = script.get('R');

					// Render the check mark polygon in the specified color
					polygon.color(stated_color(m_check_color));

					Coord min_size = min(area_check.size().width_(), area_check.size().height_());
					polygon.zoom_((min_size << 6)/resolution);

					UIManager::renderer()->draw(polygon, Point(area_check.position().x_(), area_check.position().y_(),false));
				}
			}
			else
			{
				// Default check mark (simple square)
				Rect::build_polygon(area_check, m_radius, 0, 0, ALL_BORDERS, stated_color(m_check_color), 0);
			}
		}
		
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
Widget * Checkbox::hovered(const Region & parent_region, const Point & position)
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
void Checkbox::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	CheckboxStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Checkbox::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	CheckboxStyle::unserialize(it);
}


/** Call back on key */
void Checkbox::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_BACKSPACE || evt.key() == (wchar_t)ReservedKey::KEY_DELETE || evt.key() == (wchar_t)ReservedKey::KEY_SPACE)
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
void Checkbox::on_click(Widget * widget, const ClickEvent & evt)
{
	m_checked = (m_checked == 0 ? 1 : 0);
	UIManager::notifier()->check(m_checked, this);
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
}


#ifdef _DEBUG
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
		done = true;
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
