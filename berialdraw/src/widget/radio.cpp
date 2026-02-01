#include "berialdraw_imp.hpp"

using namespace berialdraw;

// ===== RadioGroup Implementation =====

RadioGroup::RadioGroup()
{
}

RadioGroup::~RadioGroup()
{
}

/** Add a radio button to the group */
void RadioGroup::add_radio(Radio * radio)
{
	if (radio)
	{
		// Check if already in group
		for (auto r : m_radios)
		{
			if (r == radio)
				return;
		}
		m_radios.push_back(radio);
		radio->set_group(this);
	}
}

/** Remove a radio button from the group */
void RadioGroup::remove_radio(Radio * radio)
{
	if (radio)
	{
		auto it = std::find(m_radios.begin(), m_radios.end(), radio);
		if (it != m_radios.end())
		{
			m_radios.erase(it);
			if (m_selected == radio)
				m_selected = nullptr;
		}
	}
}

/** Select a specific radio button in the group, deselecting others */
void RadioGroup::select(Radio * radio)
{
	if (radio)
	{
		// Deselect previously selected radio
		if (m_selected && m_selected != radio)
		{
			m_selected->m_selected = false;
			UIManager::invalidator()->dirty(m_selected, Invalidator::REDRAW);
		}

		// Select new radio
		m_selected = radio;
		radio->m_selected = true;
		UIManager::invalidator()->dirty(radio, Invalidator::REDRAW);
	}
}

/** Get the currently selected radio button */
Radio * RadioGroup::selected() const
{
	return m_selected;
}


// ===== Radio Implementation =====

Radio::Radio(Widget * parent):
	Widget("radio", parent, sizeof(Radio))
{
	UIManager::styles()->apply(m_classname, (CommonStyle*)this);
	UIManager::styles()->apply(m_classname, (WidgetStyle*)this);
	UIManager::styles()->apply(m_classname, (TextStyle*)this);
	UIManager::styles()->apply(m_classname, (BorderStyle*)this);
	UIManager::styles()->apply(m_classname, (RadioStyle*)this);
	bind(this, &Radio::on_key);
	bind(this, &Radio::on_click);
}

Radio::~Radio()
{
	if (m_group)
	{
		m_group->remove_radio(this);
	}
}

/** Set the radio group for this button */
void Radio::set_group(RadioGroup * group)
{
	m_group = group;
}

/** Get the radio group for this button */
RadioGroup * Radio::get_group() const
{
	return m_group;
}

/** Check if radio button is selected */
bool Radio::is_selected() const
{
	return m_selected;
}

/** Copy all styles of the radio button */
void Radio::copy(const Radio & radio)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&radio);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&radio);
	*((TextStyle*)this) = *(TextStyle*)(&radio);
	*((BorderStyle*)this) = *(BorderStyle*)(&radio);
	*((RadioStyle*)this) = *(RadioStyle*)(&radio);
}

/** Copy all styles of the radio button */
void Radio::copy(const Radio * radio)
{
	if(radio)
	{
		copy(*radio);
	}
}

/** Return the size of content without margins */
Size Radio::content_size()
{
	Size result = m_radio_size;

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
		if (m_text_size.height_() > m_radio_size.height_())
		{
			result.height_(m_text_size.height_());
		}
	}
	return result;
}

/** Place all widget in area */
void Radio::place(const Area & area, bool in_layout)
{
	if (m_size.is_width_undefined() && m_size.is_height_undefined() && m_extend != Extend::EXTEND_NONE && 
		m_position.is_x_undefined() && m_position.is_y_undefined())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place the icon
	m_radio_foreclip = m_foreclip;

	// If text not empty
	if (m_text.size() > 0)
	{
		Margin marg;

		m_text_backclip = m_foreclip;
		marg.left_(m_radio_size.width_() + padding().left_());
		place_in_layout(m_text_backclip, m_text_size, marg, EXTEND_NONE, m_text_foreclip, (Align)(m_text_align | ALIGN_BOTTOM));
	
		m_radio_foreclip = m_text_foreclip;
		m_radio_foreclip.size(m_radio_size);
		Coord move_y = (m_radio_size.height_() > m_text_size.height_() ? 0-((m_radio_size.height_() - m_text_size.height_())>>1) : ((m_text_size.height_()-m_radio_size.height_())>>1));
		m_radio_foreclip.position().move_(0-(m_radio_size.width_() + padding().left_()), move_y);
		m_radio_foreclip.position().nearest_pixel();
	}
}

void Radio::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		// Create an area for just the radio (not including text)
		Area area_box(m_radio_foreclip);
		area_box.size().set_(m_radio_size.width_(), m_radio_size.height_());

		// Draw radio box
		Rect::build_focused_polygon(area_box, 
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color),
			stated_color(m_border_color),
			Color::TRANSPARENT,
			stated_color(m_focus_color),
			m_focused);

		// Draw radio indicator if selected
		if (m_selected)
		{
			Area area_radio(area_box);
			area_radio.size().decrease_(m_radio_padding << 1, m_radio_padding << 1);
			area_radio.position().move_(m_radio_padding, m_radio_padding);

			// Parse and draw the radio indicator sketch using VectorScript
			if (m_radio_sketch.size() > 0)
			{
				Polygon polygon(0);
				VectorsScript script(m_radio_sketch.c_str(), polygon);
				
				if (script.parse() == VectorsScript::SUCCESS)
				{
					Coord resolution = script.get('R');

					// Render the radio indicator polygon in the specified color
					polygon.color(stated_color(m_radio_color));

					Coord min_size = min(area_radio.size().width_(), area_radio.size().height_());
					polygon.zoom_((min_size << 6)/resolution);

					UIManager::renderer()->draw(polygon, Point(area_radio.position().x_(), area_radio.position().y_(),false));
				}
			}
			else
			{
				// Default radio indicator (simple circle/filled area)
				Rect::build_polygon(area_radio, m_radius, 0, 0, ALL_BORDERS, stated_color(m_radio_color), 0);
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
Widget * Radio::hovered(const Region & parent_region, const Point & position)
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
void Radio::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
	RadioStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Radio::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
	RadioStyle::unserialize(it);
}


/** Call back on key */
void Radio::on_key(Widget * widget, const KeyEvent & evt)
{
	if (m_focused)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_SPACE)
			{
				if (m_group)
				{
					m_group->select(this);
				}
				else
				{
					m_selected = true;
					UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
				}
			}
		}
	}
}

/** Call back on click */
void Radio::on_click(Widget * widget, const ClickEvent & evt)
{
	if (m_group)
	{
		m_group->select(this);
	}
	else
	{
		m_selected = true;
	}
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
}


#ifdef _DEBUG
void Radio::test1()
{
	Window window;
		window.position(64, 10);
		window.size(200, 470);
		window.color(Color::WHITE);

	Column * grid = new Column(&window);
	RadioGroup group;
	
	int row = 0;
	Radio * radio;

	// Group 1: Basic radios
	radio = new Radio(grid);
		group.add_radio(radio);
		group.select(radio);

	radio = new Radio(grid);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->radio_size(24, 24);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->radio_size(32, 32);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->color(Color::BLUE);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		radio->radio_size(16, 16);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->radio_size(24, 24);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->radio_size(32, 32);
		radio->thickness(2);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		group.add_radio(radio);

	Label * label = new Label(grid);
		label->text(" ");
		label->cell(row++, 0);

	//while(1) UIManager::desktop()->dispatch();
	UIManager::desktop()->dispatch("$(ui.tests)/out/radio1.svg");
}

void Radio::test2()
{
	// Test with text in layout
	Window window;
		window.position(0,0);
		window.size(400,200);
		window.color(Color::WHITE);

	Column * col = new Column(&window);
	
	RadioGroup group;
	
		// Radio without text
		Radio * radio1 = new Radio(col);
			radio1->text("");
			radio1->margin(10);
			group.add_radio(radio1);
			group.select(radio1);
		
		// Radio with text
		Radio * radio2 = new Radio(col);
			radio2->text("Option 1");
			radio2->margin(10);
			group.add_radio(radio2);
		
		// Radio with text and custom styling
		Radio * radio3 = new Radio(col);
			radio3->text("Option 2_|");
			radio3->text_color(Color::GREEN);
			radio3->font_size(20);
			radio3->margin(10);
			group.add_radio(radio3);

	//while(1) UIManager::desktop()->dispatch();	
	UIManager::desktop()->dispatch("$(ui.tests)/out/radio2.svg");
}

void Radio::test3()
{
	// Test absolute positioning with text
	Window window;

	RadioGroup group;

	Pane * pane = new Pane(&window);
		pane->size(350,50);
		pane->position(100,50);
		pane->color(Color::LIGHT_GRAY);

	// Radio with absolute position and text
	Radio * radio1 = new Radio(&window);
		radio1->position(100, 50);
		radio1->size(350, 50);
		radio1->text("Absolute position with text");
		radio1->text_color(Color::BLUE);
		radio1->font_size(16);
		radio1->color(Color::LIGHT_BLUE);
		radio1->border_color(Color::BLUE);
		radio1->thickness(2);
		group.add_radio(radio1);
		group.select(radio1);

	// Radio with absolute position without text
	Radio * radio2 = new Radio(&window);
		radio2->position(20, 80);
		radio2->text("");
		radio2->color(Color::LIGHT_GREEN);
		radio2->border_color(Color::GREEN);
		radio2->thickness(2);
		group.add_radio(radio2);

	// Radio with absolute position and different text
	Radio * radio3 = new Radio(&window);
		radio3->position(20, 130);
		radio3->size(350, 50);
		radio3->text("Another absolute radio");
		radio3->text_color(Color::RED);
		radio3->font_size(14);
		radio3->color(Color::LIGHT_RED);
		radio3->border_color(Color::RED);
		radio3->thickness(2);
		group.add_radio(radio3);

	//while(1) UIManager::desktop()->dispatch();	
	UIManager::desktop()->dispatch("$(ui.tests)/out/radio3.svg");
}

void Radio::test4()
{
	Window window;

	Column * column = new Column(&window);
	Radio * radio;
		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(5);

		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(25);

		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(55);

		radio = new Radio(column);
		radio->text("Radio");
		radio->font_size(255);

	UIManager::desktop()->dispatch("$(ui.tests)/out/radio4.svg");
}

void Radio::test()
{
	test4();
	test3();
	test2();
	test1();
}
#endif
