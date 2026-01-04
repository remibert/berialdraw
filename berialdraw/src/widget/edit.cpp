#include "berialdraw_imp.hpp"

using namespace berialdraw;

Edit::Edit(Widget * parent):
	Widget("edit", parent, sizeof(Edit)),
	Entry(&m_text),
	m_text_box()
{
	m_position = m_position;
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
	UIManager::styles()->apply(m_classname, (EditStyle*)this);
	bind(this, &Edit::on_key);
	bind(this, &Edit::on_select);
	bind(this, &Edit::on_click);
	m_edited = 1;
}

Edit::~Edit()
{
}

/** Copy all styles of the edit */
void Edit::copy(const Edit & edit)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&edit);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&edit);
	*((BorderStyle*)this) = *(BorderStyle*)(&edit);
	*((TextStyle*)this)   = *(TextStyle*)(&edit);
	*((EditStyle*)this)   = *(EditStyle*)(&edit);
}

/** Copy all styles of the edit */
void Edit::copy(const Edit * edit)
{
	if(edit)
	{
		copy(*edit);
	}
}

Size Edit::content_size()
{
	Size result;
	select_font();

	if (m_edited == 1)
	{
		m_edited = 0;
		select_all();
		entry_align((Align)m_text_align);
	}

	if(m_text_modified)
	{
		Size char_size = m_font->text_size(" ");

		// Adapt the size of text according to its size specified
		m_text_size.set_(char_size.width_() * m_max_columns, char_size.height_() * m_max_lines);
		m_text_modified = 0;
	}

	result = m_text_size;
	result.height_(result.height_()+ padding().bottom_() + padding().top_() + m_focus_gap +m_focus_gap);
	result.width_(result.width_()  + padding().left_()   + padding().right_() + m_focus_gap + m_focus_gap);

	return result;
}

void Edit::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place rectangle
	place_in_area(area, in_layout);

	// If absolute place
	if (in_layout == false)
	{
		Area backclip = m_foreclip;
		Margin marg;
		Size size(content_size());

		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			size.height_(m_foreclip.height_());
		}
		else
		{
			size.width_(m_foreclip.width_());
		}
		place_in_layout(backclip, size, marg, EXTEND_NONE, m_foreclip, (Align)m_align);
	}

	// Place text
	m_text_backclip = m_foreclip;
	m_text_backclip.decrease(padding());

	// If text area greater than the max columns authorised
	if (m_text_backclip.width_() > m_text_size.width_())
	{
		// Reduce size to the max columns
		m_text_backclip.width_(m_text_size.width_());
	}
	
	// If text area greater than the max lines authorised
	if (m_text_backclip.height_() > m_text_size.height_())
	{
		// Reduce size to the max lines
		m_text_backclip.height_(m_text_size.height_());
	}

	Margin marg;

	// Place the text and compute the size of text
	place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, text_align());

	// Adapt the fore clipping to the size of edit field
	m_foreclip = m_text_foreclip;
	m_foreclip.increase(padding());

	// Adapt the back clipping to the size of edit field
	m_backclip = m_foreclip;
	m_backclip.increase(margin());
}

void Edit::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Clipping background
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);

		String display;
		uint32_t txt_col = stated_color(m_text_color);
		
		if (m_input->size() == 0 && m_place_holder != 0)
		{
			display = *m_place_holder;
			txt_col = stated_color(m_place_holder_color);
		}
		else
		{
			display = *m_input;
		}

		// If mask selected
		if (m_mask)
		{
			MaskValidator validator(*m_mask);
			validator.display(m_text, display);
		}

		// If it is a password
		if (m_password && m_text.size() != 0)
		{
			// Hide all characters
			for (uint32_t i = 0; i < display.count(); i++)
			{
				if (display.get(i) != '\n')
				{
					display.replace('*',i);
				}
			}
		}

		m_text_box.parse(m_text_foreclip, *m_font, display, m_cursor_position, m_selection_start, m_selection_end, (Align)m_text_align);
		
		Rect::build_focused_polygon(m_foreclip, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color), 
			stated_color(m_border_color),
			Color::TRANSPARENT, 
			stated_color(m_focus_color),
			m_focused);

		Widget::paint(region);

		// Get clipping text area
		Area text_clip(m_text_backclip);

		// Add cursor width and height
		Margin cursor_margin(1,1,1,1);
		text_clip.increase(cursor_margin);

		// Clipping text area
		region.intersect(text_clip);
		UIManager::renderer()->region(region);

		// Shift to show cursor
		m_text_box.text_shift(m_cursor_shift, m_text_foreclip);

		// Move to text to display cursor location
		Point text_shift(m_text_foreclip.position());
		text_shift.move(m_cursor_shift);

		m_text_box.paint(m_cursor_shift, *m_font.get(), display, m_text_foreclip.position(), m_text_backclip, txt_col,
			focused() ? stated_color(m_cursor_color) : Color::TRANSPARENT, 
			focused() ? stated_color(m_select_color) : Color::TRANSPARENT,
			(TypingMode)m_typing_mode == TypingMode::INSERTION);
	}
}


/** Get the widget hovered */
Widget * Edit::hovered(const Region & parent_region, const Point & position)
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

/** Call back on key */
void Edit::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			on_key_down(evt.key(), evt.modifier());
			m_text_modified = 1;
			UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
		}
	}
}

/** Treat the click on widget, used to change the focus */
void Edit::on_select(Widget * widget, const SelectEvent & evt)
{
	if (m_text.size() > 0)
	{
		uint32_t to  =UINT32_MAX;

		select_font();

		if (evt.status() == SelectEvent::SELECT_START)
		{
			m_select_from = m_text_box.cursor_position(evt.position(), m_text_foreclip, m_cursor_shift, *m_font.get(), m_text, (Align)m_text_align);
		}
		else if (evt.status() == SelectEvent::SELECT_PROGRESS || evt.status() == SelectEvent::SELECT_END)
		{
			to   = m_text_box.cursor_position(evt.position(),   m_text_foreclip, m_cursor_shift, *m_font.get(), m_text, (Align)m_text_align);
		}
	
		if (to != UINT32_MAX && m_select_from != UINT32_MAX)
		{
			select_text(m_select_from,to);
		}

		if (evt.status() == SelectEvent::SELECT_END)
		{
			m_select_from = UINT32_MAX;
		}
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

/** Call back on click */
void Edit::on_click(Widget * widget, const ClickEvent & evt)
{
	if (m_text.size() > 0)
	{
		uint32_t position  =UINT32_MAX;

		select_font();
		position = m_text_box.cursor_position(evt.position(), m_text_foreclip, m_cursor_shift, *m_font.get(), m_text, (Align)m_text_align);
	
		if (position != UINT32_MAX)
		{
			select_text(position,position);
		}
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

/** Serialize the content of widget into json */
void Edit::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	EditStyle::serialize(it);

	if (m_mask)
	{
		it["mask"]      = *m_mask;
	}
}

/** Unserialize the content of widget from json */
void Edit::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	EditStyle::unserialize(it);

	String mask;
	mask   = (String)it["text"];
	if (mask.size())
	{
		if (m_mask)
		{
			delete m_mask;
		}
		m_mask = new String(mask);
	}
}

#ifdef _DEBUG
void Edit::test1()
{
	Window window;
		window.position(160,0);
		window.size(320,480);

		Grid * grid = new Grid(&window);
			grid->id(60001);
		int row = 0;

		Label * name = new Label(grid);
			name->cell(row++,0);
			name->text("First name");
			name->text_align(Align::ALIGN_LEFT);
			name->margin(10,10,0,10);
			name->id(60002);

		Edit * edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,0);
			edit->text_align(Align::ALIGN_LEFT);
			edit->margin(10,5);
			edit->max_lines(3);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->id(60003);

		name = new Label(grid);
			name->cell(row++,0);
			name->text("Last name");
			name->text_align(Align::ALIGN_LEFT);
			name->margin(10,10,0,10);
			name->id(60004);

		edit = new Edit(grid);
			edit->text("BERTHILOT");
			edit->cell(row++,0);
			edit->text_align(Align::ALIGN_LEFT);
			edit->margin(10,5);
			edit->id(60005);

		Button * button = new Button(grid);
			button->text("Register\nEncore\nune\nligne");
			button->cell(row++,0);
			button->id(60006);
			button->extend(Extend::EXTEND_NONE);
			button->margin(20,20);
			button->align(Align::ALIGN_RIGHT);
			button->text_align(Align::ALIGN_LEFT);

		Label * spacer = new Label(grid);
			spacer->text("eee");
			spacer->font_size(4);
			spacer->color(RED);
			spacer->cell(row++,0);
			spacer->size_policy(SizePolicy::ENLARGE_HEIGHT);
			spacer->id(60007);

		Keyboard * keyboard = new Keyboard(grid);
			keyboard->cell(row++,0);
			keyboard->size_policy(SizePolicy::ENLARGE_ALL);
			keyboard->id(60008);

	int i = 1;
	int id = 0;
	for (i = 0; i < 180; i+= 18)
	{
		spacer->font_size(i);
		String name;
		name.print("$(ui.tests)/out/edit1_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
}

void Edit::test2()
{
//UIManager::notifier()->log();
	Window window;
		window.position(0,0);
		window.size(480,480);

		Grid * grid = new Grid(&window);
		int row = 0;
		int column = 0;
		Label * label;
		Edit  * edit;

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Left insert");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_LEFT);
			edit->typing_mode(TypingMode::INSERTION);
			edit->max_lines(2);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Center insert");
			label->text_align(Align::CENTER);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::CENTER);
			edit->typing_mode(TypingMode::INSERTION);
			edit->max_lines(2);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Right insert");
			label->text_align(Align::ALIGN_RIGHT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_RIGHT);
			edit->typing_mode(TypingMode::INSERTION);
			edit->max_lines(2);

		row = 0;
		column++;

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Left replace");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_LEFT);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->max_lines(2);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Center replace");
			label->text_align(Align::CENTER);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::CENTER);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->max_lines(2);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Right replace");
			label->text_align(Align::ALIGN_RIGHT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_RIGHT);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->max_lines(2);
//UIManager::desktop()->mainloop();
	String script(
	"["
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
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
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
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':'shift','character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':'shift','character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':127,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':127,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':127,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':127,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':127,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':127,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':275,'y':  4,'state':'move'},"
		"{'type':'touch','x':246,'y': 52,'state':'move'},"
		"{'type':'touch','x':243,'y': 72,'state':'move'},"
		"{'type':'touch','x':243,'y': 78,'state':'move'},"
		"{'type':'touch','x':305,'y':138,'state':'move'},"
		"{'type':'touch','x':475,'y':264,'state':'move'},"
	"]");
	UIManager::notifier()->play_script(script, "$(ui.tests)/out/edit2_%d.svg");
}

void Edit::test3()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.id(123);

		Grid * grid = new Grid(&window);
		int row = 0;
		int column = 0;
		Label * label;
		Edit  * edit;

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Left insert");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_LEFT);
			edit->typing_mode(TypingMode::INSERTION);
			edit->max_lines(2);
			edit->id(1);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Center insert");
			label->text_align(Align::CENTER);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::CENTER);
			edit->typing_mode(TypingMode::INSERTION);
			edit->max_lines(2);
			edit->id(2);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Right insert");
			label->text_align(Align::ALIGN_RIGHT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_RIGHT);
			edit->typing_mode(TypingMode::INSERTION);
			edit->max_lines(2);
			edit->id(3);

		row = 0;
		column++;

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Left replace");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_LEFT);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->max_lines(2);
			edit->id(4);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Center replace");
			label->text_align(Align::CENTER);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::CENTER);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->max_lines(2);
			edit->id(5);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Right replace");
			label->text_align(Align::ALIGN_RIGHT);

		edit = new Edit(grid);
			edit->text("Lorem ipsum dolor sit amet, \nconsectetur adipiscing elit,\n sed do eiusmod tempor incididunt ut labore\n et dolore magna aliqua.");
			edit->cell(row++,column);
			edit->text_align(Align::ALIGN_RIGHT);
			edit->typing_mode(TypingMode::REPLACEMENT);
			edit->max_lines(2);
			edit->id(6);

	String script(
	"["
		"{'type':'touch','x':163,'y': 87,'state':'down'},"
		"{'type':'touch','x':163,'y': 87,'state':'up'},"
		"{'type':'touch','x': 20,'y': 23,'state':'down'},"
		"{'type':'touch','x': 20,'y': 23,'state':'up'},"
		"{'type':'touch','x':225,'y': 25,'state':'down'},"
		"{'type':'touch','x':225,'y': 25,'state':'up'},"
		"{'type':'touch','x':231,'y':113,'state':'down'},"
		"{'type':'touch','x':231,'y':113,'state':'up'},"
		"{'type':'touch','x': 21,'y':112,'state':'down'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x': 21,'y':112,'state':'up'},"
		"{'type':'touch','x':246,'y': 24,'state':'down'},"
		"{'type':'touch','x':246,'y': 24,'state':'up'},"
		"{'type':'touch','x':459,'y':116,'state':'down'},"
		"{'type':'touch','x':459,'y':116,'state':'up'},"
		"{'type':'touch','x': 17,'y':128,'state':'down'},"
		"{'type':'touch','x': 17,'y':128,'state':'up'},"
		"{'type':'touch','x':230,'y':220,'state':'down'},"
		"{'type':'touch','x':230,'y':220,'state':'up'},"
		"{'type':'touch','x':241,'y':131,'state':'down'},"
		"{'type':'touch','x':241,'y':131,'state':'up'},"
		"{'type':'touch','x':458,'y':331,'state':'down'},"
		"{'type':'touch','x':458,'y':331,'state':'up'},"
		"{'type':'touch','x':309,'y':177,'state':'down'},"
		"{'type':'touch','x':309,'y':177,'state':'up'},"
		"{'type':'touch','x':427,'y':175,'state':'down'},"
		"{'type':'touch','x':427,'y':175,'state':'up'},"
		"{'type':'touch','x':273,'y':176,'state':'down'},"
		"{'type':'touch','x':273,'y':176,'state':'up'},"
		"{'type':'touch','x':128,'y':293,'state':'down'},"
		"{'type':'touch','x':128,'y':293,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':117,'y':124,'state':'down'},"
		"{'type':'touch','x':117,'y':124,'state':'up'},"
		"{'type':'touch','x':111,'y':290,'state':'down'},"
		"{'type':'touch','x':111,'y':290,'state':'up'},"
		"{'type':'touch','x':115,'y':451,'state':'down'},"
		"{'type':'touch','x':115,'y':451,'state':'up'},"
		"{'type':'touch','x':354,'y':132,'state':'down'},"
		"{'type':'touch','x':354,'y':132,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':354,'y':294,'state':'down'},"
		"{'type':'touch','x':354,'y':294,'state':'up'},"
		"{'type':'touch','x':356,'y':454,'state':'down'},"
		"{'type':'touch','x':356,'y':454,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x': 10,'y':109,'state':'down'},"
		"{'type':'touch','x':187,'y':136,'state':'up'},"
		"{'type':'touch','x':299,'y':107,'state':'down'},"
		"{'type':'touch','x':381,'y':131,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		
		"{'type':'touch','x': 57,'y':268,'state':'down'},"
		"{'type':'touch','x':153,'y':298,'state':'up'},"
		"{'type':'touch','x':299,'y':270,'state':'down'},"
		"{'type':'touch','x':397,'y':293,'state':'up'},"
		"{'type':'touch','x': 64,'y':427,'state':'down'},"
		"{'type':'touch','x':143,'y':449,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'touch','x':300,'y':425,'state':'down'},"
		"{'type':'touch','x':363,'y':451,'state':'up'},"
		"{'type':'touch','x':202,'y':129,'state':'down'},"
		"{'type':'touch','x': 78,'y':111,'state':'up'},"
		"{'type':'touch','x':184,'y':294,'state':'down'},"
		"{'type':'touch','x': 36,'y':268,'state':'up'},"
		"{'type':'touch','x':183,'y':450,'state':'down'},"
		"{'type':'touch','x': 42,'y':426,'state':'up'},"
		"{'type':'touch','x':409,'y':130,'state':'down'},"
		"{'type':'touch','x':282,'y':106,'state':'up'},"
		"{'type':'touch','x':417,'y':291,'state':'down'},"
		"{'type':'touch','x':281,'y':265,'state':'up'},"
		"{'type':'touch','x':392,'y':451,'state':'down'},"
		"{'type':'touch','x':277,'y':424,'state':'up'},"
		"{'type':'touch','x':182,'y':111,'state':'down'},"
		"{'type':'touch','x':182,'y':111,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8676,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8676,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':8676,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8676,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8676,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8676,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8592,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x': 39,'y':433,'state':'down'},"
		"{'type':'touch','x': 39,'y':433,'state':'up'},"
		"{'type':'touch','x':177,'y':291,'state':'down'},"
		"{'type':'touch','x':177,'y':291,'state':'up'},"
		"{'type':'key','key':8677,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8677,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8593,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':8595,'state':'up'  ,'modifier':''     ,'character':' '},"
		"{'type':'touch','x':209,'y':271,'state':'down'},"
		"{'type':'touch','x':209,'y':271,'state':'up'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/edit3_%d.svg");
}

void Edit::test4()
{
	Window window;
		window.size(480,480);
		Grid * grid = new Grid(&window);
		int row = 0;
		int column = 0;
		Label * label;
		Edit  * edit;
		window.color(Color::WHITE);

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Time");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("00:00:00");
			edit->cell(row++,column);
			edit->mask("%02[0..23]d:%02[0..59]d:%02[0..59]d");

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Temperature");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("-100");
			edit->cell(row++,column);
			edit->mask("%[-99..99]d.%02[0..99]d");

		label = new Label(grid);
			label->cell(row++,column);
			label->text("Ip address");
			label->text_align(Align::ALIGN_LEFT);

		edit = new Edit(grid);
			edit->text("0.0.0.0");
			edit->cell(row++,column);
			edit->mask("(%3[0..255]d.%3[0..255]d.%3[0..255]d.%3[0..255]d)");
		
		Label * spacer = new Label(grid);
			spacer->text("");
			spacer->font_size(4);
			spacer->color(RED);
			spacer->cell(row++,0);
			spacer->size_policy(SizePolicy::ENLARGE_HEIGHT);
	String script(
	"["
		"{'type':'key','key': 49,'state':'down','modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'up'  ,'modifier':''     ,'character':'1'},"
		"{'type':'key','key': 56,'state':'down','modifier':''     ,'character':'8'},"
		"{'type':'key','key': 56,'state':'up'  ,'modifier':''     ,'character':'8'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key': 49,'state':'down','modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'up'  ,'modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'down','modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'up'  ,'modifier':''     ,'character':'1'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key': 45,'state':'down','modifier':''     ,'character':'-'},"
		"{'type':'key','key': 45,'state':'up'  ,'modifier':''     ,'character':'-'},"
		"{'type':'key','key': 57,'state':'down','modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'up'  ,'modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'down','modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'up'  ,'modifier':''     ,'character':'9'},"
		"{'type':'key','key': 49,'state':'down','modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'up'  ,'modifier':''     ,'character':'1'},"
		"{'type':'key','key': 48,'state':'down','modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'up'  ,'modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'down','modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'up'  ,'modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'down','modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'up'  ,'modifier':''     ,'character':'0'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key': 49,'state':'down','modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'up'  ,'modifier':''     ,'character':'1'},"
		"{'type':'key','key': 50,'state':'down','modifier':''     ,'character':'2'},"
		"{'type':'key','key': 50,'state':'up'  ,'modifier':''     ,'character':'2'},"
		"{'type':'key','key': 56,'state':'down','modifier':''     ,'character':'8'},"
		"{'type':'key','key': 56,'state':'up'  ,'modifier':''     ,'character':'8'},"
		"{'type':'key','key': 48,'state':'down','modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'up'  ,'modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'down','modifier':''     ,'character':'0'},"
		"{'type':'key','key': 48,'state':'up'  ,'modifier':''     ,'character':'0'},"
		"{'type':'key','key': 49,'state':'down','modifier':''     ,'character':'1'},"
		"{'type':'key','key': 49,'state':'up'  ,'modifier':''     ,'character':'1'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"

		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"

		"{'type':'key','key': 50,'state':'down','modifier':''     ,'character':'2'},"
		"{'type':'key','key': 50,'state':'up'  ,'modifier':''     ,'character':'2'},"
		"{'type':'key','key': 52,'state':'down','modifier':''     ,'character':'4'},"
		"{'type':'key','key': 52,'state':'up'  ,'modifier':''     ,'character':'4'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key': 57,'state':'down','modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'up'  ,'modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'down','modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'up'  ,'modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'down','modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'up'  ,'modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'down','modifier':''     ,'character':'9'},"
		"{'type':'key','key': 57,'state':'up'  ,'modifier':''     ,'character':'9'},"
		"{'type':'key','key':  9,'state':'down','modifier':''     ,'character':'I'},"
		"{'type':'key','key':  9,'state':'up'  ,'modifier':''     ,'character':'I'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
		"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
		"{'type':'key','key': 50,'state':'down','modifier':''     ,'character':'2'},"
		"{'type':'key','key': 50,'state':'up'  ,'modifier':''     ,'character':'2'},"
		"{'type':'key','key': 53,'state':'down','modifier':''     ,'character':'5'},"
		"{'type':'key','key': 53,'state':'up'  ,'modifier':''     ,'character':'5'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key': 53,'state':'down','modifier':''     ,'character':'5'},"
		"{'type':'key','key': 53,'state':'up'  ,'modifier':''     ,'character':'5'},"
		"{'type':'key','key': 51,'state':'down','modifier':''     ,'character':'3'},"
		"{'type':'key','key': 51,'state':'up'  ,'modifier':''     ,'character':'3'},"
		"{'type':'key','key': 53,'state':'down','modifier':''     ,'character':'5'},"
		"{'type':'key','key': 53,'state':'up'  ,'modifier':''     ,'character':'5'},"
		"{'type':'key','key': 54,'state':'down','modifier':''     ,'character':'6'},"
		"{'type':'key','key': 54,'state':'up'  ,'modifier':''     ,'character':'6'},"
		"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
		"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
	"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/edit4_%d.svg");
}

void Edit::test5()
{
	Window window;
		window.size(160,180);
		window.color(Color::TRANSPARENT);
		Label * label;
		Edit  * edit;

		Pane * pane = new Pane(&window);
			pane->border_color(Color::ASH_GRAY);
			pane->thickness(2);
			pane->radius(8);
			pane->color(Color::WHITE_GRAY);
			pane->margin(2);
		
		Column * grid = new Column(pane);

		label = new Label(grid);
			label->text("Login");
			label->text_color(Color::GRAY_DOLPHIN);

		edit = new Edit(grid);
			edit->text("");
			edit->id(123);
			edit->margin(2,10,2,10);

		label = new Label(grid);
			label->text("Password");
			label->text_color(Color::GRAY_DOLPHIN);

		edit = new Edit(grid);
			edit->text("");
			edit->password(true);
			edit->margin(2,10,2,10);

		Label * spacer = new Label(grid);
			spacer->text("");
			spacer->font_size(4);
			spacer->size_policy(SizePolicy::ENLARGE_HEIGHT);

		Button * connect = new Button(grid);
			connect->text("Connect");
			connect->color(Color::DARK_GAINSBORO);
			connect->border_color(Color::BLACK_CAT);
			connect->margin(2,30,10,30);

	#define TOUCH_EVT 1
	#define KEY_EVT 2
	const struct Event
	{
		uint32_t type;
		uint32_t par1;
		uint32_t par2;
		uint32_t par3;
		uint32_t par4;
	} events [] =
	{
		{KEY_EVT,  'R',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'R',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'e',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'e',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'm',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'm',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'i',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'i',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,   9,KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,   9,KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'B',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'B',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'E',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'E',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'R',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'R',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'T',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'T',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,  'H',KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,  'H',KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,   9,KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,   9,KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{KEY_EVT,9208,KeyEvent::KEY_DOWN,KeyEvent::MODIFIER_NONE },{KEY_EVT,9208,KeyEvent::KEY_UP,KeyEvent::MODIFIER_NONE },
		{0}
	};

	int i =0;
	int id = 0;
//while(true)
	//UIManager::desktop()->mainloop();
	UIManager::desktop()->dispatch();
	String test_name;
	while (events[i].type != 0)
	{
		test_name = "";
		if (events[i].type == KEY_EVT)
		{
			if (events[i].par1 != (wchar_t) ReservedKey::KEY_PAUSE)
			{
				UIManager::notifier()->key(events[i].par1, (KeyEvent::KeyState)events[i].par2, (KeyEvent::Modifier)events[i].par3);
			}
			else
			{
				if (events[i].par2 == KeyEvent::KEY_DOWN)
				{
					test_name.print("$(ui.tests)/out/edit5_%d.svg", ++id);
				}
			}
		}
		else if (events[i].type == TOUCH_EVT)
		{
			UIManager::notifier()->touch(events[i].par1, events[i].par2, (TouchEvent::TouchState)events[i].par3);
		}
		UIManager::desktop()->dispatch(test_name);
		i++;
	}
	assert(edit->text() == "BERTH");
}

void Edit::test6()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(80,150);
		window.size(320,180);

	Dim row = 0;
	Grid * scroll_layout = new Grid(&window);
		Edit * edit = new Edit(scroll_layout);
			edit->cell(row++,0);
			edit->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi.");
			edit->place_holder("Enter text");

		edit = new Edit(scroll_layout);
			edit->cell(row++,0);
			edit->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi.");
			edit->place_holder("Enter text");
		
		String script(
		"["
			"{'type':'key','key':8594,'state':'down','modifier':''     ,'character':' '},"
			"{'type':'key','key':8594,'state':'up'  ,'modifier':''     ,'character':' '},"
			"{'type':'touch','x':220,'y':  4,'state':'move'},"
			"{'type':'touch','x':241,'y': 73,'state':'move'},"
			"{'type':'touch','x':257,'y':141,'state':'move'},"
			"{'type':'touch','x':258,'y':149,'state':'move'},"
			"{'type':'touch','x':253,'y':147,'state':'move'},"
			"{'type':'touch','x':247,'y':160,'state':'move'},"
			"{'type':'touch','x':244,'y':170,'state':'move'},"
			"{'type':'touch','x':238,'y':178,'state':'move'},"
			"{'type':'touch','x':238,'y':194,'state':'move'},"
			"{'type':'touch','x':243,'y':201,'state':'move'},"
			"{'type':'touch','x':247,'y':203,'state':'move'},"
			"{'type':'touch','x':247,'y':200,'state':'down'},"
			"{'type':'touch','x':246,'y':200,'state':'move'},"
			"{'type':'touch','x':239,'y':200,'state':'move'},"
			"{'type':'touch','x':229,'y':199,'state':'move'},"
			"{'type':'touch','x':219,'y':198,'state':'move'},"
			"{'type':'touch','x':209,'y':198,'state':'move'},"
			"{'type':'touch','x':199,'y':198,'state':'move'},"
			"{'type':'touch','x':189,'y':196,'state':'move'},"
			"{'type':'touch','x':181,'y':196,'state':'move'},"
			"{'type':'touch','x':167,'y':196,'state':'move'},"
			"{'type':'touch','x':147,'y':196,'state':'move'},"
			"{'type':'touch','x':128,'y':197,'state':'move'},"
			"{'type':'touch','x':121,'y':197,'state':'move'},"
			"{'type':'touch','x':115,'y':197,'state':'move'},"
			"{'type':'touch','x':107,'y':197,'state':'move'},"
			"{'type':'touch','x': 97,'y':199,'state':'move'},"
			"{'type':'touch','x': 90,'y':200,'state':'move'},"
			"{'type':'touch','x': 86,'y':200,'state':'move'},"
			"{'type':'touch','x': 80,'y':200,'state':'move'},"
			"{'type':'touch','x': 76,'y':200,'state':'move'},"
			"{'type':'touch','x': 74,'y':200,'state':'move'},"
			"{'type':'touch','x': 78,'y':198,'state':'move'},"
			"{'type':'touch','x': 81,'y':197,'state':'move'},"
			"{'type':'touch','x': 85,'y':197,'state':'move'},"
			"{'type':'touch','x': 90,'y':197,'state':'move'},"
			"{'type':'touch','x':114,'y':200,'state':'move'},"
			"{'type':'touch','x':132,'y':202,'state':'move'},"
			"{'type':'touch','x':138,'y':202,'state':'move'},"
			"{'type':'touch','x':142,'y':202,'state':'move'},"
			"{'type':'touch','x':146,'y':202,'state':'move'},"
			"{'type':'touch','x':150,'y':202,'state':'move'},"
			"{'type':'touch','x':156,'y':202,'state':'move'},"
			"{'type':'touch','x':163,'y':202,'state':'move'},"
			"{'type':'touch','x':174,'y':202,'state':'move'},"
			"{'type':'touch','x':182,'y':202,'state':'move'},"
			"{'type':'touch','x':190,'y':202,'state':'move'},"
			"{'type':'touch','x':195,'y':202,'state':'move'},"
			"{'type':'touch','x':199,'y':202,'state':'move'},"
			"{'type':'touch','x':207,'y':201,'state':'move'},"
			"{'type':'touch','x':233,'y':199,'state':'move'},"
			"{'type':'touch','x':264,'y':199,'state':'move'},"
			"{'type':'touch','x':291,'y':199,'state':'move'},"
			"{'type':'touch','x':308,'y':200,'state':'move'},"
			"{'type':'touch','x':316,'y':200,'state':'move'},"
			"{'type':'touch','x':333,'y':200,'state':'move'},"
			"{'type':'touch','x':350,'y':200,'state':'move'},"
			"{'type':'touch','x':360,'y':201,'state':'move'},"
			"{'type':'touch','x':372,'y':202,'state':'move'},"
			"{'type':'touch','x':383,'y':202,'state':'move'},"
			"{'type':'touch','x':389,'y':201,'state':'move'},"
			"{'type':'touch','x':393,'y':200,'state':'move'},"
			"{'type':'touch','x':402,'y':200,'state':'move'},"
			"{'type':'touch','x':407,'y':200,'state':'move'},"
			"{'type':'touch','x':403,'y':200,'state':'move'},"
			"{'type':'touch','x':397,'y':200,'state':'move'},"
			"{'type':'touch','x':391,'y':200,'state':'move'},"
			"{'type':'touch','x':365,'y':200,'state':'move'},"
			"{'type':'touch','x':341,'y':198,'state':'move'},"
			"{'type':'touch','x':320,'y':197,'state':'move'},"
			"{'type':'touch','x':299,'y':196,'state':'move'},"
			"{'type':'touch','x':254,'y':190,'state':'move'},"
			"{'type':'touch','x':220,'y':184,'state':'move'},"
			"{'type':'touch','x':200,'y':181,'state':'move'},"
			"{'type':'touch','x':191,'y':179,'state':'move'},"
			"{'type':'touch','x':191,'y':178,'state':'move'},"
			"{'type':'touch','x':191,'y':178,'state':'move'},"
			"{'type':'touch','x':182,'y':178,'state':'move'},"
			"{'type':'touch','x':159,'y':180,'state':'move'},"
			"{'type':'touch','x':135,'y':184,'state':'move'},"
			"{'type':'touch','x':124,'y':187,'state':'move'},"
			"{'type':'touch','x':120,'y':187,'state':'move'},"
			"{'type':'touch','x':116,'y':187,'state':'move'},"
			"{'type':'touch','x':110,'y':188,'state':'move'},"
			"{'type':'touch','x':106,'y':188,'state':'move'},"
			"{'type':'touch','x': 99,'y':188,'state':'move'},"
			"{'type':'touch','x': 91,'y':189,'state':'move'},"
			"{'type':'touch','x': 83,'y':189,'state':'move'},"
			"{'type':'touch','x': 75,'y':189,'state':'move'},"
			"{'type':'touch','x': 73,'y':189,'state':'move'},"
			"{'type':'touch','x': 77,'y':189,'state':'move'},"
			"{'type':'touch','x': 91,'y':190,'state':'move'},"
			"{'type':'touch','x':122,'y':196,'state':'move'},"
			"{'type':'touch','x':151,'y':201,'state':'move'},"
			"{'type':'touch','x':177,'y':205,'state':'move'},"
			"{'type':'touch','x':180,'y':205,'state':'move'},"
			"{'type':'touch','x':175,'y':205,'state':'move'},"
			"{'type':'touch','x':170,'y':204,'state':'move'},"
			"{'type':'touch','x':166,'y':204,'state':'move'},"
			"{'type':'touch','x':163,'y':203,'state':'move'},"
			"{'type':'touch','x':160,'y':201,'state':'move'},"
			"{'type':'touch','x':160,'y':201,'state':'up'},"
			"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
			"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
			"{'type':'touch','x':162,'y':205,'state':'move'},"
			"{'type':'touch','x':167,'y':216,'state':'move'},"
			"{'type':'touch','x':174,'y':231,'state':'move'},"
			"{'type':'touch','x':175,'y':236,'state':'move'},"
			"{'type':'touch','x':175,'y':246,'state':'move'},"
			"{'type':'touch','x':173,'y':253,'state':'move'},"
			"{'type':'touch','x':173,'y':258,'state':'move'},"
			"{'type':'touch','x':173,'y':264,'state':'move'},"
			"{'type':'touch','x':173,'y':277,'state':'move'},"
			"{'type':'touch','x':173,'y':284,'state':'move'},"
			"{'type':'touch','x':174,'y':287,'state':'move'},"
			"{'type':'touch','x':176,'y':287,'state':'move'},"
			"{'type':'touch','x':178,'y':286,'state':'down'},"
			"{'type':'touch','x':178,'y':286,'state':'up'},"
			"{'type':'touch','x':178,'y':286,'state':'down'},"
			"{'type':'touch','x':179,'y':286,'state':'move'},"
			"{'type':'touch','x':191,'y':286,'state':'move'},"
			"{'type':'touch','x':208,'y':286,'state':'move'},"
			"{'type':'touch','x':222,'y':286,'state':'move'},"
			"{'type':'touch','x':232,'y':286,'state':'move'},"
			"{'type':'touch','x':247,'y':286,'state':'move'},"
			"{'type':'touch','x':267,'y':284,'state':'move'},"
			"{'type':'touch','x':297,'y':284,'state':'move'},"
			"{'type':'touch','x':323,'y':287,'state':'move'},"
			"{'type':'touch','x':344,'y':291,'state':'move'},"
			"{'type':'touch','x':360,'y':292,'state':'move'},"
			"{'type':'touch','x':367,'y':292,'state':'move'},"
			"{'type':'touch','x':371,'y':292,'state':'move'},"
			"{'type':'touch','x':376,'y':292,'state':'move'},"
			"{'type':'touch','x':384,'y':292,'state':'move'},"
			"{'type':'touch','x':392,'y':292,'state':'move'},"
			"{'type':'touch','x':398,'y':292,'state':'move'},"
			"{'type':'touch','x':402,'y':293,'state':'move'},"
			"{'type':'touch','x':406,'y':294,'state':'move'},"
			"{'type':'touch','x':408,'y':295,'state':'move'},"
			"{'type':'touch','x':403,'y':295,'state':'move'},"
			"{'type':'touch','x':401,'y':295,'state':'move'},"
			"{'type':'touch','x':401,'y':295,'state':'move'},"
			"{'type':'touch','x':393,'y':295,'state':'move'},"
			"{'type':'touch','x':379,'y':295,'state':'move'},"
			"{'type':'touch','x':356,'y':295,'state':'move'},"
			"{'type':'touch','x':326,'y':294,'state':'move'},"
			"{'type':'touch','x':278,'y':293,'state':'move'},"
			"{'type':'touch','x':225,'y':287,'state':'move'},"
			"{'type':'touch','x':205,'y':287,'state':'move'},"
			"{'type':'touch','x':193,'y':287,'state':'move'},"
			"{'type':'touch','x':178,'y':287,'state':'move'},"
			"{'type':'touch','x':151,'y':287,'state':'move'},"
			"{'type':'touch','x':120,'y':287,'state':'move'},"
			"{'type':'touch','x': 91,'y':287,'state':'move'},"
			"{'type':'touch','x': 75,'y':287,'state':'move'},"
			"{'type':'touch','x': 73,'y':286,'state':'move'},"
			"{'type':'touch','x': 85,'y':286,'state':'move'},"
			"{'type':'touch','x':112,'y':286,'state':'move'},"
			"{'type':'touch','x':170,'y':290,'state':'move'},"
			"{'type':'touch','x':190,'y':293,'state':'move'},"
			"{'type':'touch','x':194,'y':294,'state':'move'},"
			"{'type':'touch','x':198,'y':294,'state':'move'},"
			"{'type':'touch','x':202,'y':294,'state':'move'},"
			"{'type':'touch','x':207,'y':294,'state':'move'},"
			"{'type':'touch','x':215,'y':294,'state':'move'},"
			"{'type':'touch','x':219,'y':294,'state':'move'},"
			"{'type':'touch','x':223,'y':294,'state':'move'},"
			"{'type':'touch','x':227,'y':294,'state':'move'},"
			"{'type':'touch','x':231,'y':294,'state':'move'},"
			"{'type':'touch','x':229,'y':294,'state':'move'},"
			"{'type':'touch','x':225,'y':294,'state':'move'},"
			"{'type':'touch','x':222,'y':294,'state':'up'},"
			"{'type':'touch','x':223,'y':294,'state':'move'},"
			"{'type':'key','key':127,'state':'down','modifier':''     ,'character':''},"
			"{'type':'key','key':127,'state':'up'  ,'modifier':''     ,'character':''},"
			"{'type':'touch','x':223,'y':294,'state':'down'},"
			"{'type':'touch','x':228,'y':294,'state':'move'},"
			"{'type':'touch','x':240,'y':293,'state':'move'},"
			"{'type':'touch','x':273,'y':289,'state':'move'},"
			"{'type':'touch','x':302,'y':289,'state':'move'},"
			"{'type':'touch','x':319,'y':292,'state':'move'},"
			"{'type':'touch','x':331,'y':292,'state':'move'},"
			"{'type':'touch','x':354,'y':293,'state':'move'},"
			"{'type':'touch','x':385,'y':296,'state':'move'},"
			"{'type':'touch','x':407,'y':300,'state':'move'},"
			"{'type':'touch','x':413,'y':301,'state':'move'},"
			"{'type':'touch','x':415,'y':301,'state':'move'},"
			"{'type':'touch','x':411,'y':300,'state':'move'},"
			"{'type':'touch','x':397,'y':299,'state':'move'},"
			"{'type':'touch','x':371,'y':298,'state':'move'},"
			"{'type':'touch','x':348,'y':298,'state':'move'},"
			"{'type':'touch','x':333,'y':297,'state':'move'},"
			"{'type':'touch','x':309,'y':297,'state':'move'},"
			"{'type':'touch','x':288,'y':297,'state':'move'},"
			"{'type':'touch','x':286,'y':297,'state':'move'},"
			"{'type':'touch','x':290,'y':296,'state':'move'},"
			"{'type':'touch','x':291,'y':296,'state':'up'},"
			"{'type':'key','key':  8,'state':'down','modifier':''     ,'character':'H'},"
			"{'type':'key','key':  8,'state':'up'  ,'modifier':''     ,'character':'H'},"
			"{'type':'key','key':8676,'state':'down','modifier':''     ,'character':' '},"
			"{'type':'key','key':8676,'state':'up'  ,'modifier':''     ,'character':' '},"
			"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
			"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
			"{'type':'touch','x':327,'y':295,'state':'move'},"
			"{'type':'touch','x':485,'y':484,'state':'move'},"
		"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/edit6_%d.svg");
}

void Edit::test7()
{
	Window window;
		window.position(0,0);
		window.size(480,100);
		Edit  * edit;
			edit = new Edit(&window);
			edit->max_columns(10);
			edit->max_lines(3);
			edit->text("0\n1\n");

		String script(
		"["
			"{'type':'key','key':0x2192,'state':'down','modifier':''     ,'character':' '},"
			"{'type':'key','key':0x2192,'state':'up'  ,'modifier':''     ,'character':' '},"
			"{'type':'key','key':9208,'state':'down','modifier':''     ,'character':' '},"
			"{'type':'key','key':9208,'state':'up'  ,'modifier':''     ,'character':' '},"
		"]");

	UIManager::notifier()->play_script(script, "$(ui.tests)/out/edit7_%d.svg");
}

void Edit::test8()
{
	//UIManager::notifier()->log();
	Window window;
		window.position(10,10);
		window.size(50,50);
		window.color(Color::LIGHT_RED);

	Dim row = 0;
	Grid * scroll_layout = new Grid(&window);
		Edit * edit = new Edit(scroll_layout);
			edit->cell(row++,0);
			edit->text("");
			edit->border_color(Color::LIGHT_GREEN);
			edit->focus_color(Color::LIGHT_BLUE);
			//edit->margin(4);
			//edit->place_holder("Enter text");

		//edit = new Edit(scroll_layout);
		//	edit->cell(row++,0);
		//	edit->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi.");
		//	edit->place_holder("Enter text");
	UIManager::desktop()->dispatch();
edit->margin(3);
UIManager::desktop()->dispatch();
edit->margin(2);
UIManager::desktop()->dispatch();
edit->margin(1);
UIManager::desktop()->dispatch();
edit->margin(0);
UIManager::desktop()->dispatch();
}

void Edit::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
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
