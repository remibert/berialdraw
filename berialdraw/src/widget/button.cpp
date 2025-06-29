#include "berialdraw_imp.hpp"

using namespace berialdraw;

Button::Button(Widget * parent):
	Widget("button", parent, sizeof(Button))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
}

Button::~Button()
{
}

void Button::copy(const Button & button)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&button);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&button);
	*((BorderStyle*)this) = *(BorderStyle*)(&button);
	*((TextStyle  *)this) = *(TextStyle  *)(&button);
}

Size Button::content_size()
{
	Size result;
	if(m_text_modified || m_font_modified)
	{
		Area area;
		select_font();
		m_text_box.parse(area, *m_font, m_text, UINT32_MAX, UINT32_MAX, UINT32_MAX, (Align)m_text_align);
		m_text_modified = m_font_modified = 0;
		m_text_size = m_text_box.content_size();
	}

	result = m_text_size;
	result.height_(result.height_()+ padding().bottom_() + padding().top_());
	result.width_(result.width_()  + padding().left_() + padding().right_());

	// Compute the children with marged size
	Size children_size = Widget::children_size();
	if (children_size.width_() > result.width_())
	{
		result.width_(children_size.width_());
	}
	if (children_size.height_() > result.height_())
	{
		result.height_(children_size.height_());
	}
	return result;
}

void Button::place(const Area & area, bool in_layout)
{
	Margin marg;

	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place button text
	m_text_backclip = m_foreclip;
	m_text_backclip.decrease(padding());
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)m_text_align);

	// Place all children
	Area backclip(m_backclip);
	Widget::place(m_foreclip,in_layout);

	// Restore backclip
	m_backclip = backclip;
}

void Button::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Draw rectangle
	region.intersect(m_backclip);

	// If button visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		Point shift;

		Rect::build_focused_polygon(m_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT, 
			stated_color(m_focus_color),
			m_focused);

		// Paint children
		Widget::paint(region);

		// Paint text
		region.intersect(m_text_backclip);
		select_font();
		UIManager::renderer()->region(region);
		m_text_box.paint(shift, *m_font.get(), m_text, m_text_foreclip.position(), m_text_backclip, stated_color(m_text_color), 0, 0, true);
	}
}

/** Get the widget hovered */
Widget * Button::hovered(const Region & parent_region, const Point & position)
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
void Button::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Button::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
}

#ifdef _DEBUG
void Button::test1()
{
	Window window;
		window.position(11,19);
		window.size(117,219);
		window.color(Color::BLONDE);

	Button * button = new Button(&window);
	Button * button1 = new Button(&window);
	Button * button2 = new Button(button1);
	new Button(button1);
	new Button(button2);
	new Button(button1);
	new Button(button2);
	Button * button3 = new Button(button2);
	new Button(button1);
	new Button(button3);
	UIManager::desktop()->dispatch();;
}

void Button::test2()
{
	Window window;
		window.position(11,19);
		window.size(117,219);
		window.color(Color::BROWN);

	Button * button = new Button(&window);
	Button * button1 = new Button(&window);
	Button * button2 = new Button(button1);
	new Button(button1);
	new Button(button2);
	new Button(button1);
	new Button(button2);
	Button * button3 = new Button(button2);
	new Button(button1);
	new Button(button3);
	delete button1;

	UIManager::desktop()->dispatch();;
}

void Button::test3()
{
	Window window;
		window.position(11,19);
		window.size(117,219);
		window.color(Color::BROWN);

	Button * button = new Button(&window);
	Button * button1 = new Button(&window);
	Button * button2 = new Button(&window);
	Button * button3 = new Button(&window);
	Button * button4 = new Button(&window);
	Button * button5 = new Button(&window);

	UIManager::desktop()->dispatch();

	delete button1;
	delete button3;
	delete button5;
	delete button4;
	delete button2;
}

void Button::test4()
{
	Button * button;

	uint32_t s = sizeof(Button);

	Window window;
		window.position(0,400);
		window.size(480,80);
		window.color(Color::YELLOW_ORANGE);

		button = new Button(&window);
		 	button->text("Bottom Right");
		 	button->position(240,0);
		  	button->min_size(230,70);
			button->padding(3,3,3,3);
			button->margin(5,5,5,5);
			button->text_align(ALIGN_BOTTOM_RIGHT);
			button->color(Color::DODGER_BLUE, 32);

		button = new Button(&window);
		 	button->text("Bottom Left");
		 	button->position(240,0);
		  	button->min_size(230,70);
			button->padding(3,3,3,3);
			button->margin(5,5,5,5);
			button->text_align(ALIGN_BOTTOM_LEFT);
			button->color(Color::DODGER_BLUE, 32);

		button = new Button(&window);
		 	button->text("Center");
		 	button->position(240,0);
		  	button->min_size(230,70);
			button->padding(3,3,3,3);
			button->margin(5,5,5,5);
			button->color(Color::DODGER_BLUE, 32);

		button = new Button(&window);
		 	button->text("Top Left");
		 	button->position(240,0);
		  	button->min_size(230,70);
			button->padding(3,3,3,3);
			button->margin(5,5,5,5);
			button->text_align(ALIGN_TOP_LEFT);
			button->color(Color::DODGER_BLUE, 32);

		button = new Button(&window);
		 	button->text("Top Right");
		 	button->position(240,0);
		  	button->min_size(230,70);
			button->padding(3,3,3,3);
			button->margin(5,5,5,5);
			button->text_align(ALIGN_TOP_RIGHT);
			button->color(Color::DODGER_BLUE, 32);

		button = new Button(&window);
			button->position(30,30);
			button->text("Press key");
			button->id(124);

	Window window2;
		window2.color(Color::RED_BLOOD);
		window2.position(0,0);
		window2.size(480,400);

		Grid * grid = new Grid(&window2);
			button = new Button(grid);
				button->id(0);
				button->text("%d",button->id());
				button->cell(0,0);
				button->text_align(Align::CENTER);
				button->extend(Extend::EXTEND_ALL);

			button = new Button(grid);
				button->id(1);
				button->text("%d",button->id());
				button->cell(0,1);
				button->text_align(Align::ALIGN_BOTTOM_RIGHT);
				button->extend(Extend::EXTEND_ALL);

			button = new Button(grid);
				button->id(2);
				button->text("%d",button->id());
				button->cell(0,2);
				button->text_align(Align::ALIGN_BOTTOM_LEFT);
				button->extend(Extend::EXTEND_ALL);

			button = new Button(grid);
				button->id(3);
				button->text("%d",button->id());
				button->cell(1,0);
				button->text_align(Align::CENTER);
				button->extend(Extend::EXTEND_HEIGHT);

			button = new Button(grid);
				button->id(4);
				button->text("%d",button->id());
				button->cell(1,1);
				button->text_align(Align::ALIGN_BOTTOM_RIGHT);
				button->extend(Extend::EXTEND_WIDTH);

			button = new Button(grid);
				button->id(5);
				button->text("%d",button->id());
				button->cell(1,2);
				button->text_align(Align::ALIGN_BOTTOM_LEFT);
				button->extend(Extend::EXTEND_NONE);

			button = new Button(grid);
				button->id(6);
				button->text("%d",button->id());
				button->cell(2,0);
				button->text_align(Align::CENTER);
				button->extend(Extend::EXTEND_ALL);

			button = new Button(grid);
				button->id(7);
				button->text("%d",button->id());
				button->cell(2,1);
				button->text_align(Align::ALIGN_TOP_RIGHT);
				button->extend(Extend::EXTEND_ALL);

			button = new Button(grid);
				button->id(8);
				button->text("%d",button->id());
				button->cell(2,2);
				button->text_align(Align::ALIGN_TOP_LEFT);
				button->extend(Extend::EXTEND_ALL);

			button = new Button(grid);
				button->id(9);
				button->text("%d",button->id());
				button->cell(3,0);
				button->text_align(Align::CENTER);
				button->extend(Extend::EXTEND_WIDTH);

			button = new Button(grid);
				button->id(10);
				button->text("%d",button->id());
				button->cell(3,1);
				button->text_align(Align::ALIGN_TOP_RIGHT);
				button->extend(Extend::EXTEND_HEIGHT);

			button = new Button(grid);
				button->id(11);
				button->text("%d",button->id());
				button->cell(3,2);
				button->text_align(Align::ALIGN_TOP_LEFT);
				button->extend(Extend::EXTEND_NONE);
	UIManager::desktop()->dispatch("test/out/button4.svg");
}

void Button::test5()
{
	Button * button;
	Window window;
		window.position(0,0);
		window.size(200,200);
		window.color(Color::WHITE);

		button = new Button(&window);
			button->position(30,30);
			button->text("Press key");
			button->id(123);

		UIManager::desktop()->dispatch("test/out/button5_1.svg");
		button->text("Press");
		UIManager::desktop()->dispatch("test/out/button5_2.svg");
		button->font_size(30);
		UIManager::desktop()->dispatch("test/out/button5_3.svg");
		button->pressed(true);
		UIManager::desktop()->dispatch("test/out/button5_4.svg");

		button->color(Color::GREEN);
		button->border_color(Color::BOTTLE_GREEN);
		button->text_color(Color::YELLOW);
		button->thickness(5);
		button->radius(10);
		button->align(CENTER);
		button->padding(25,25,25,25);

	button = dynamic_cast<Button*>(window.search(123));
	if(button)
	{
		button->text("Casted");
		button->color(Color::RED);
		button->border_color(Color::RED_BLOOD);
	}
	UIManager::desktop()->dispatch("test/out/button5_5.svg");
}

void Button::test6()
{
	Window window;
		window.position(0,0);
		window.size(UIManager::renderer()->size());
		Grid * grid = new Grid(&window);
			Pane * rect;
				uint16_t row = 0, column = 0;
				for (uint16_t hue = 0; hue < 360; hue += 15)
				{
					for (uint16_t light = 0; light < 100; light += 5)
					{
						uint32_t color  = Hsl::to_rgb(hue,100,light);
						rect = new Pane(grid);
						rect->cell(row, column);
						rect->color(color);
						rect = new Pane(grid);
						rect->cell(row+1, column);
						rect->color(window.pressed_color(color,true));
						column ++;
					}
					row += 2;
					column = 0;
				}
	UIManager::desktop()->dispatch("test/out/button6.svg");
}

void test_keyboard(Grid * grid, String & keyboard)
{
#define KEY_LETTER      "\x01"
#define KEY_NUMERIC     "\x02"
#define KEY_SHIFT       "\xE2\xAC\x86" // 0x2B06
#define KEY_ENTER       "\xE2\x86\xA9" // 0x21A9
#define KEY_BACKSPACE   "\xE2\x8C\xAB" // 0x232B
#define KEY_ARROW_LEFT  "\xE2\x86\x90" // 0x2190
#define KEY_ARROW_UP    "\xE2\x86\x91" // 0x2191
#define KEY_ARROW_RIGHT "\xE2\x86\x92" // 0x2192
#define KEY_ARROW_DOWN  "\xE2\x86\x93" // 0x2193
#define KEY_EURO        "\xE2\x82\xAC" // 0x20AC
#define KEY_POUND       "\xC2\xA3"     // 0xA3
#define KEY_YEN         "\xC2\xA5"     // 0xA5
	Grid * keyline;
	Button * button;
	int line = 0;
	int column = 0;
	
	wchar_t key;
	keyline = new Grid(grid);
	keyline->cell(0,0);
	for(int i = 0; i < (int)keyboard.count(); i++)
	{
		key = keyboard.get(i);

		if(key == 0x1F)
		{
			Label * label = new Label(keyline);
			label->cell(0, column);
			column ++;
		}
		else if(key == '\n')
		{
			line++;
			keyline = new Grid(grid);
			keyline->cell(line, 0);
			column = 0;
		}
		else
		{
			button = new Button(keyline);
			button->font_size(28);
			//button->font_size(18);
			if      (key == Utf8::to_int(KEY_LETTER))
			{
				button->font_size(18);
				button->text(" ABC ");
			}
			else if (key == Utf8::to_int(KEY_NUMERIC))
			{
				button->font_size(18);
				button->text(" 123 ");
			}
			else if ( key == Utf8::to_int(KEY_ENTER))
			{
				button->text(" %s ",KEY_ENTER);
			}
			else if (key ==  ' ')
			{
				//button->font_size(28);
				button->text(" ");
				button->size_policy(SizePolicy::ENLARGE_WIDTH);
			}
			else
			{
				button->text(key);
			}
			button->cell(0, column);
			button->radius(7);
			button->thickness_(32);
			button->margin(3, 3, 3, 3);
			button->border_color(0xFF7ABBE8);
			button->color(0xFFE7FCFF);
			button->text_color(0xFF0060AF);
			column ++;
		}
	}
}

void Button::test7()
{
	const char * keymap = 
		"{\n"
			"'keys':\n"
			"{\n"
				"'radius':7,\n"
				"'thickness':1,\n"
				"'margins':[3,3,3,3],\n"
				"'font':{'size':28},\n"
				"'colors':\n"
				"{\n"
					"'border'    :0xFF7ABBE8,\n"
					"'background':0xFFE7FCFF,\n"
					"'text'      :0xFF0060AF,\n"
				"}\n"
			"},\n"
			"'specialtkeys':\n"
			"{\n"
				"'radius':7,\n"
				"'thickness':1,\n"
				"'margins':[3,3,3,3],\n"
				"'font':{'size':40},\n"
				"'keys':'\x01\x02',\n"
				"'colors':\n"
				"{\n"
					"'border'    :0xFF7ABBE8,\n"
					"'background':0xFFE7FCFF,\n"
					"'text'      :0xFF0060AF,\n"
				"}\n"
			"},\n"
			"'mappings':\n"
			"[\n"
				"{'shift':{'text':null, 'goto':1}, 'caps':{'text':'123', 'goto':3}, 'mapping':'qwertyuiop\\nasdfghjkl\\'\\n\\u2B06zxcvbnm\\u232B\\n\x02 \\u21A9'},\n"
				"{'shift':{'text':null, 'goto':0}, 'caps':{'text':'123', 'goto':3}, 'mapping':'QWERTYUIOP\\nASDFGHJKL\\'\\n\\u2B06ZXCVBNM\\u232B\\n\x02 \\u21A9'},\n"
				"{'shift':{'text':null, 'goto':3}, 'caps':{'text':'ABC', 'goto':0}, 'mapping':'1234567890\\n-/:;()\\u20AC&@\"\\n\\u2B06.,?!\\'\\u232B\\n\x01 \\u21A9'},\n"
				"{'shift':{'text':null, 'goto':2}, 'caps':{'text':'ABC', 'goto':0}, 'mapping':'[]{}#\x25^*+=\\n_\\\\|~<>$\\u00A3\\u00A5\"\\n\\u2B06.,?!\\'\\u232B\\n\x01 \\u21A9'}\n"
			"]\n"
		"}";
/*


	01234567
	89ABCDEF
	X  <  O


	01234
	56789
	X < O
*/
	Json json;
	json.unserialize(keymap);

	String keyboard_minus(json["mappings"][0]["mapping"]);
	String keyboard_caps (json["mappings"][1]["mapping"]);
	String keyboardNum1 (json["mappings"][2]["mapping"]);
	String keyboardNum2 (json["mappings"][3]["mapping"]);
	String keyboard_digit("123\n456\n789\n" KEY_BACKSPACE "0" KEY_ENTER);

	String keyboard_hexa("123AB\n456CD\n789EF\n" KEY_BACKSPACE "0" KEY_ENTER);

	String keyboard_arrow("\x1F" KEY_ARROW_UP "\x1F" "\n" KEY_ARROW_LEFT KEY_ARROW_DOWN KEY_ARROW_RIGHT);

	String & keyboard = keyboard_minus;
	Window window;
		window.position(0,0);
		window.size(480,240);
		Grid * grid = new Grid(&window);

	//while(1)
	{
		test_keyboard(grid, keyboard_minus);
		UIManager::desktop()->dispatch("test/out/button7_1.svg");

		{
			Json json;
			
			JsonIterator win(json);
			window.serialize(win);
			{
				File out;
				out.open("test/out/button7_1.json","w");
				json.serialize(out,1);
			}
		}

		grid->clear(); 
		test_keyboard(grid, keyboard_caps);
		UIManager::desktop()->dispatch("test/out/button7_2.svg");
		grid->clear(); 

		test_keyboard(grid, keyboardNum1);
		UIManager::desktop()->dispatch("test/out/button7_3.svg");
		grid->clear(); 

		test_keyboard(grid, keyboardNum2);
		UIManager::desktop()->dispatch("test/out/button7_4.svg");
		grid->clear(); 
	}
}

class TestButton
{
public:
	
	void on_key(Widget * widget, const KeyEvent & evt)
	{
		bd_printf("Key %c %d\n",evt.key(),(int)evt.state());
	}

	void on_event(Widget * widget, const TouchEvent & evt)
	{
		Label * label = dynamic_cast<Label*>(widget->root()->search(1));
		label->text("%s %d,%d:%d touch",widget->classname(), evt.position().x(),evt.position().y(),evt.state());
	}

	void on_click(Widget * widget, const ClickEvent & evt)
	{
		Label * label = dynamic_cast<Label*>(widget->root()->search(1));
		label->text("%s clicked",widget->classname());
	}

	void on_quit(Widget * widget, const ClickEvent & evt)
	{
		m_quit = true;
	}

	bool m_quit = false;
};

void Button::test8()
{
	TestButton t;
	Window window;
		window.bind(&t, &TestButton::on_key);
		window.position(13,17);
		window.size(450,319);
		window.color(Color::BROWN,32);
		window.bind(&t, &TestButton::on_event);

	Grid * grid = new Grid(&window);

		Button * button = new Button(grid);
			button->text("Hello");
			//button->position(19,23);
			button->cell(0,0);
			button->bind(&t, &TestButton::on_event);
			button->bind(&t, &TestButton::on_click);

		Canvas * canvas = new Canvas(grid);
			// canvas->size(40,40);
			canvas->extend(EXTEND_ALL);
			//canvas->position(20,20);
			canvas->color(Color::LIGHT_BEIGE);
			canvas->cell(0,1);
			canvas->bind(&t, &TestButton::on_event);
			
			Point center(10, 10);

			{
				Cross * cross = new Cross(canvas);
				cross->color(Color::BLACK);
				cross->thickness(3);
				cross->position(center);
				cross->radius(10);
			}

		berialdraw::Pane * rect = new berialdraw::Pane(grid);
			rect->cell(1,0);
			rect->color(Color::GREEN);
			rect->bind(&t, &TestButton::on_event);

		Label * label = new Label(grid);
			label->cell(1,1);
			label->text_color(Color::ORANGE);
			label->text("label");
			//label->font_size(10);
			label->bind(&t, &TestButton::on_event);
			label->id(1);

		Button * quit = new Button(grid);
			quit->text("quit");
			quit->cell(2,0);
			quit->bind(&t, &TestButton::on_quit);

	const struct Key
	{
	Coord x;
	Coord y;
	TouchEvent::TouchState state;
	} events [] =
	{
		{163,87,TouchEvent::TOUCH_DOWN},
		{163,87,TouchEvent::TOUCH_UP},
		{20,23,TouchEvent::TOUCH_DOWN},
		{20,23,TouchEvent::TOUCH_UP},
		{225,25,TouchEvent::TOUCH_DOWN},
		{225,25,TouchEvent::TOUCH_UP},
		{231,113,TouchEvent::TOUCH_DOWN},
		{231,113,TouchEvent::TOUCH_UP},
		{21,112,TouchEvent::TOUCH_DOWN},
		{21,112,TouchEvent::TOUCH_UP},
		{246,24,TouchEvent::TOUCH_DOWN},
		{246,24,TouchEvent::TOUCH_UP},
		{459,116,TouchEvent::TOUCH_DOWN},
		{459,116,TouchEvent::TOUCH_UP},
		{17,128,TouchEvent::TOUCH_DOWN},
		{17,128,TouchEvent::TOUCH_UP},
		{230,220,TouchEvent::TOUCH_DOWN},
		{230,220,TouchEvent::TOUCH_UP},
		{241,131,TouchEvent::TOUCH_DOWN},
		{241,131,TouchEvent::TOUCH_UP},
		{458,331,TouchEvent::TOUCH_DOWN},
		{458,331,TouchEvent::TOUCH_UP},
		{309,177,TouchEvent::TOUCH_DOWN},
		{309,177,TouchEvent::TOUCH_UP},
		{427,175,TouchEvent::TOUCH_DOWN},
		{427,175,TouchEvent::TOUCH_UP},
		{273,176,TouchEvent::TOUCH_DOWN},
		{273,176,TouchEvent::TOUCH_UP},

		{128,293,TouchEvent::TOUCH_DOWN},
		{128,293,TouchEvent::TOUCH_UP},
		{0,0,TouchEvent::TOUCH_DOWN},
	};


	int i = 0;	
	const char * label_control[] = 
	{
		"",
		"label",
		"button 163,87:1 touch",
		"button clicked",
		"button 20,23:1 touch",
		"button clicked",
		"button 225,25:1 touch",
		"button clicked",
		"button 231,113:1 touch",
		"button clicked",
		"button 21,112:1 touch",
		"button clicked",
		"canvas 246,24:1 touch",
		"canvas 246,24:0 touch",
		"canvas 459,116:1 touch",
		"canvas 459,116:0 touch",
		"pane 17,128:1 touch",
		"pane 17,128:0 touch",
		"pane 230,220:1 touch",
		"pane 230,220:0 touch",
		"window 241,131:1 touch",
		"window 241,131:0 touch",
		"window 458,331:1 touch",
		"window 458,331:0 touch",
		"label 309,177:1 touch",
		"label 309,177:0 touch",
		"label 427,175:1 touch",
		"label 427,175:0 touch",
		"label 273,176:1 touch",
		"label 273,176:0 touch",
		"label 273,176:0 touch",
		"label 273,176:0 touch",
		"",
	};
	while (t.m_quit == false)
	{
		if (label->text() != "")
		{
			if (events[i].x != 0 && events[i].y != 0)
			{
				if (i>=16)
					i = i;
				UIManager::notifier()->touch(events[i].x, events[i].y, events[i].state);			i++;
				//bd_printf("%d,\"%s\",\n",i, label->text().c_str());
				assert(label->text() == label_control[i]);

			}
		}
		UIManager::desktop()->dispatch();
	}
}

void Button::test9()
{
	Window window;
		window.position(13,17);
		window.size(450,319);
		window.color(Color::BROWN,32);

	Grid * grid = new Grid(&window);
		Button * button = new Button(grid);
			button->cell(0,0);
			button->text("Hello\nWorld\nHave a nice day");
			button->extend(Extend::EXTEND_NONE);

		button = new Button(grid);
			button->cell(0,1);
			button->text("Hello\nWorld\nHave a nice day");
			button->text_align(Align::ALIGN_LEFT);
			button->extend(Extend::EXTEND_NONE);

		button = new Button(grid);
			button->cell(1,0);
			button->text("Hello\nWorld\nHave a nice day");
			button->text_align(Align::ALIGN_RIGHT);
			button->extend(Extend::EXTEND_NONE);

		button = new Button(grid);
			button->cell(1,1);
			button->text("Hello\nWorld\nHave a nice day");
			button->text_align(Align::CENTER);
			button->extend(Extend::EXTEND_NONE);

	UIManager::desktop()->dispatch("test/out/button9_1.svg");
}

static Button * new_menu_item(Widget * parent, uint32_t hue, const char * menu, const char * icon = 0)
{
	Button * button;
	Label * label;
	Icon * icn;

	uint32_t color = Hsl::to_balanced_color(hue % 360, 100, 90);
	

	button = new Button(parent);
		//button->text("%d",hue);
		button->text(menu);
		button->text_color(Color::GRAY);
		button->text_align(Align::ALIGN_LEFT);
		button->font_size(20);

		button->color(Color::WHITE);
		button->border_color(Color::GRAY);
		button->sides(BOTTOM_SIDE);
		button->extend(Extend::EXTEND_WIDTH);

		button->margin(1);
		// button->padding(15);
		button->thickness(1);
		button->focus_thickness(0);

		button->radius(0);
		if (icon)
		{
			//button->padding(50,0);
			icn = new Icon(button);
				icn->filename(icon);
				icn->align(Align::ALIGN_LEFT);
				icn->color(color);
				icn->icon_color(Color::WHITE);
				icn->margin(3);
				//icn->padding(10);
				icn->radius(100);
				icn->zoom_((64+64));
				Size icon_size = icn->content_size();
				button->padding(0,icon_size.width()+5,0,0);
		}
		label = new Label(button);
			label->text(">");
			label->text_color(Color::GRAY);
			label->text_align(Align::ALIGN_RIGHT);
			label->font_size(button->font_size());
			
	return button;
}

void Button::test10()
{
	//UIManager::notifier()->log();
	Window window;
		window.size(480,480);

	ScrollView * scroll = new ScrollView(&window);
		scroll->align(Align::ALIGN_TOP);
		scroll->extend(Extend::EXTEND_HEIGHT);
		// scroll->margin(10,0);

	Column * main = new Column(scroll);

	uint32_t hue = 0;

	for (Directory directory("resources/icons"); directory.exist(); directory.next())
	{
		if (directory.match("*.icn"))
		{
			Button * button = new_menu_item(main, hue, directory.filename(), directory.full_path());
			hue += 23;
		}
	}

	String script(
	"["
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':'shift','character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':'shift','character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");
	UIManager::notifier()->play_script(script, "test/out/button10_%d.svg");
}

void Button::test11()
{
	Button * button;
	Window window;
		Grid * grid = new Grid(&window);

	for (uint16_t line = 0; line < 4; line ++)
	{
		for (uint16_t column = 0; column < 4; column++)
		{
			button = new Button(grid);
			button->cell(line, column);
			button->color(Color::BISQUE);
			button->border_color(Color::RED);
			button->thickness(line+1);
			button->focus_thickness(1);
			button->focus_color(Color::RED);
			button->radius(10);
			button->focus_gap(column);
			button->focus_color(Color::BROWN);
			button->text("thickness=%d\ngap=%d",line+1,column);
			button->size_policy(SizePolicy::SHRINK_ALL);
			button->text_color(Color::RED_BLOOD);
			button->margin(10);
		}
	}
	
	String script(
	"["
		"{'type':'snapshot','filename':'test/out/button11_%d.svg'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"

	"]");
	UIManager::notifier()->play_script(script, "");
}

void Button::test12()
{
	Button * button;
	Window window;
		window.position(0,0);
		window.size(300,300);
		window.color(Color::WHITE);

		button = new Button(&window);
			button->position(120,120);
			button->text("Press");
			button->id(123);
			button->font_size(40);
			button->color(Color::LIGHT_DAY_BLUE,128);
			button->focus_color(Color::RED);
			button->border_color(Color::OCEAN_BLUE,128);
			button->text_color(Color::NEW_MIDNIGHT_BLUE);
			button->thickness(3);
			button->radius(20);
			button->align(CENTER);
			button->margin(10);
			button->padding(10);

	int id = 0;
	String name;

	for (Dim gap = 0; gap <= 3; gap ++)
	{
		for (Dim thickness = 0; thickness <= 3; thickness ++)
		{
			button->focus_gap(gap);
			button->focus_thickness(thickness);

			button->sides(ALL_SIDES);
			name.print("test/out/button12_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);

			button->sides(BOTTOM_SIDE|RIGHT_SIDE|ROUNDED_EXTREMITY);
			name.print("test/out/button12_%d.svg", ++id);
			UIManager::desktop()->dispatch(name);
		}
	}
}

void Button::test13()
{
#if 0
	Window window;

		Column * column = new Column(&window);
		Row * row = new Row(column);
			Button * buttonRef;
			Button * button;
	

			buttonRef = new Button(row);
				buttonRef->text("One");
				buttonRef->sides(Sides::LEFT_SIDE|BOTTOM_SIDE|RIGHT_SIDE | Sides::RECTANGULAR_EXTREMITY);
				buttonRef->color(Color::LIGHT_DAY_BLUE,128);
				buttonRef->border_color(Color::OCEAN_BLUE,128);
				buttonRef->text_color(Color::NEW_MIDNIGHT_BLUE);
				buttonRef->thickness(1);
				buttonRef->radius(6);
				buttonRef->align(CENTER);
				buttonRef->margin(20);

				buttonRef->focus_color(Color::OCEAN_BLUE);
				buttonRef->focus_gap(1);
				buttonRef->focus_thickness(2);


			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Two");
				button->sides(Sides::LEFT_SIDE|TOP_SIDE|RIGHT_SIDE | Sides::RECTANGULAR_EXTREMITY);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Three");
				button->sides(Sides::LEFT_SIDE|BOTTOM_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Four");
				button->sides(Sides::RIGHT_SIDE|BOTTOM_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);

			row = new Row(column);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Five");
				button->sides(LEFT_SIDE|BOTTOM_SIDE | Sides::RECTANGULAR_EXTREMITY);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Six");
				button->sides(Sides::LEFT_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Seven");
				button->sides(Sides::RIGHT_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Height");
				button->sides(Sides::RIGHT_SIDE|BOTTOM_SIDE | Sides::RECTANGULAR_EXTREMITY);


		Pane * pane = new Pane(column);
			pane->color(Color::WHITE);
			pane->size_policy(SizePolicy::ENLARGE_ALL);
	while(1)
		UIManager::desktop()->dispatch("test/out/button13.svg");
buttonRef->text("O\ne");
	while(1)
		UIManager::desktop()->dispatch();
#endif
#if 1
	UIManager::styles()->style("pearl");            // Select the style pearl
	UIManager::colors()->appearance("light");       // Select the light appearance
	UIManager::colors()->theme(Color::THEME_LIME);  // Select the color theme

	Window window;
	Column * col;
		Column * column = new Column(&window);
		Row * row = new Row(column);
			Button * buttonRef;
			Button * button;

			buttonRef = new Button(row);
				buttonRef->text("One");
				buttonRef->sides(Sides::RIGHT_SIDE|BOTTOM_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);
				buttonRef->margin(10,10,10,2);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Two");
				button->sides(Sides::BOTTOM_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);
				button->margin(2,10);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Three");
				button->sides(Sides::BOTTOM_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);
				button->margin(2,10);

			button = new Button(row);
				button->copy(*buttonRef);
				button->text("Four");
				buttonRef->sides(Sides::LEFT_SIDE|BOTTOM_SIDE|TOP_SIDE | Sides::RECTANGULAR_EXTREMITY);
				button->margin(10,2,10,10);

		Pane * paneOne = new Pane(column);
			paneOne->color(Color::WHITE);
			paneOne->size_policy(SizePolicy::ENLARGE_ALL);
			paneOne->radius(10);
			paneOne->thickness(2);
			paneOne->margin(5);
			paneOne->hidden(true);

			col = new Column(paneOne);
				button = new Button(col);
				button->text("one");


		Pane * paneTwo = new Pane(column);
			paneTwo->color(Color::WHITE);
			paneTwo->size_policy(SizePolicy::ENLARGE_ALL);
			paneTwo->radius(10);
			paneTwo->thickness(2);
			paneTwo->margin(5);
			//paneTwo->hidden(true);

			col = new Column(paneTwo);
				button = new Button(col);
				button->text("two");



	while(1)
		UIManager::desktop()->dispatch("test/out/button13.svg");
#endif
}

void Button::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
//		test13();
		test12();
		test11();
		test10();
		test9();
		test8();
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
