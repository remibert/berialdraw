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
void Radio::copy(const Radio & radio_)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&radio_);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&radio_);
	*((BorderStyle*)this) = *(BorderStyle*)(&radio_);
	*((RadioStyle*)this) = *(RadioStyle*)(&radio_);
}

/** Copy all styles of the radio button */
void Radio::copy(const Radio * radio_)
{
	if(radio_)
	{
		copy(*radio_);
	}
}

/** Return the size of content without marges */
Size Radio::content_size()
{
	return m_radio_size;
}


void Radio::place(const Area & area, bool in_layout)
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

void Radio::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		Dim thickness = (m_focused == 0 ? m_thickness: m_thickness + (m_focus_thickness<<6));

		UIManager::renderer()->region(region);

		Area area_box(m_foreclip);

		// Draw radio box
		Rect::build_focused_polygon(m_foreclip, 
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
			Area area_radio(m_foreclip);
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
	BorderStyle::serialize(it);
	RadioStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Radio::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
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

	Grid * grid = new Grid(&window);
	RadioGroup group;
	
	int row = 0;
	Radio * radio;

	// Group 1: Basic radios
	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		group.add_radio(radio);
		group.select(radio);

	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		radio->radio_size(24, 24);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		radio->radio_size(32, 32);
		group.add_radio(radio);

	// Group 2: Extended radios
	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		radio->color(Color::BLUE);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		radio->radio_size(16, 16);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		radio->radio_size(24, 24);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		group.add_radio(radio);

	radio = new Radio(grid);
		radio->cell(row++, 0);
		radio->id(row);
		radio->radio_size(32, 32);
		radio->thickness(2);
		radio->extend(Extend::EXTEND_WIDTH);
		radio->size_policy(SizePolicy::SHRINK_HEIGHT);
		group.add_radio(radio);

	Label * label = new Label(grid);
		label->text(" ");
		label->cell(row++, 0);

	while(1) UIManager::desktop()->dispatch();
}

void Radio::test()
{
	test1();
}
#endif
