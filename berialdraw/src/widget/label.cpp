#include "berialdraw_imp.hpp"

using namespace berialdraw;

Label::Label(Widget * parent):
	Widget("label", parent, sizeof(Label))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
}

Label::~Label()
{
}

/** Copy all styles of the label */
void Label::copy(const Label & label)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&label);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&label);
	*((TextStyle*)this)   = *(TextStyle*)(&label);
}

/** Copy all styles of the label */
void Label::copy(const Label * label)
{
	if(label)
	{
		copy(*label);
	}
}

Size Label::content_size()
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

void Label::place(const Area & area, bool in_layout)
{
	if (m_text_align != ALIGN_DEFAULT && m_align == ALIGN_DEFAULT)
	{
		m_align     = m_text_align;
		m_text_align = ALIGN_DEFAULT;
	}
	Margin marg;

	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	Area text_backclip = m_foreclip;
	text_backclip.decrease(padding());
	place_in_layout(text_backclip, m_text_size, marg, EXTEND_NONE, m_foreclip, (Align)m_text_align);
}

void Label::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		select_font();
		UIManager::renderer()->region(region);

		Point shift;
		m_text_box.paint(shift, *m_font.get(), m_text, m_foreclip.position(), m_backclip, stated_color(m_text_color), 0, 0, true);
	}
}

/** Get the widget hovered */
Widget * Label::hovered(const Region & parent_region, const Point & position)
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
void Label::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Label::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
}


#ifdef _DEBUG
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
		usleep(1000);
		UIManager::desktop()->dispatch();

		if (s % 18 == 0)
		{
			String name;
			name.print("${ui.tests}/out/label1_%d.svg", ++id);
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

	UIManager::desktop()->dispatch("${ui.tests}/out/label2_1.svg");
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
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
