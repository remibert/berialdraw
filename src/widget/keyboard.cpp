#include "berialdraw_imp.hpp"

using namespace berialdraw;

Keyboard::Keyboard(Widget * parent):
	Widget("keyboard", parent, sizeof(Keyboard))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (TextStyle*)this);

	m_mappings = UIManager::styles()->mappings(m_classname);
	m_keys     = UIManager::styles()->keys(m_classname);
	m_grid = new Grid(this);
	m_grid->id(40000);

	// Select the first mapping in configuration
	create("");
}

Keyboard::~Keyboard()
{
}

/** Copy all styles of the keyboard */
void Keyboard::copy(const Keyboard & keyboard)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&keyboard);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&keyboard);
	*((BorderStyle*)this) = *(BorderStyle*)(&keyboard);
	*((TextStyle*)this)   = *(TextStyle*)(&keyboard);
}

/** Copy all styles of the keyboard */
void Keyboard::copy(const Keyboard * keyboard)
{
	if(keyboard)
	{
		copy(*keyboard);
	}
}

void Keyboard::place(const Area & area, bool in_layout)
{
	place_in_area(area, in_layout);
	Widget::place(area, in_layout);
}

/** Create a mapping according to its name */
void Keyboard::create(const String & name)
{
	Grid * keyline;
	Button * button;
	int line = 0;
	int column = 0;

	if (m_mappings)
	{
		uint16_t grid_id = 40001;
		m_mappingSelected = m_mappings->select(name);

		if (m_mappingSelected)
		{
			m_grid->clear();
		
			wchar_t key;
			const Key * key_style;

			keyline = new Grid(m_grid);
			keyline->id(grid_id ++);
			keyline->cell(0,0);

			uint32_t count = m_mappingSelected->mapping().count();

			for(int i = 0; i < (int)count; i++)
			{
				key = m_mappingSelected->mapping().get(i);

				if(key == '\t')
				{
					Label * label = new Label(keyline);
					label->cell(0, column);
					label->text(" ");
					label->size_policy(SHRINK_WIDTH);
					label->font_size_(m_font_size.width_()>>2,m_font_size.height_()>>2);
					column ++;
				}
				else if(key == '\r')
				{
					line++;
					keyline = new Grid(m_grid);
					keyline->id(grid_id ++);
					keyline->cell(line, 0);
					column = 0;
				}
				else
				{
					if (m_keys)
					{
						key_style = m_keys->key(key, m_mappingSelected->name());
					}
					else
					{
						key_style = 0;
					}
					button = new Button(keyline);
					button->CommonStyle::set(*this);
					button->WidgetStyle::set(*this);
					button->BorderStyle::set(*this);
					button->TextStyle::set(*this);

					if (key_style)
					{
						button->CommonStyle::set(*key_style);
						button->WidgetStyle::set(*key_style);
						button->BorderStyle::set(*key_style);
						button->TextStyle::set(*key_style);
					}
					else
					{
						button->text(key);
					}
					button->pressable(true);
					button->id(key);
					button->cell(0, column);
					button->bind(this, &Keyboard::on_touch_key);
					column ++;
				}
			}
		}
	}
}

/** Serialize the content of widget into json */
void Keyboard::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Keyboard::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
}


/** Get the widget hovered */
Widget * Keyboard::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;
	Widget* child = m_grid;

	// Scan all widget children
	while (child && result == 0)
	{
		result = child->hovered(parent_region, position);
		child = child->next();
	}
	return result;
}

/** Call back on touch key */
void Keyboard::on_touch_key(Widget * widget, const TouchEvent & evt)
{
	const Key * key = m_keys->key(widget->id(), m_mappingSelected->name());
	bool send_key = true;

	if (key)
	{
		if (key->to() != "")
		{
			if (evt.state() == TouchEvent::TOUCH_DOWN)
			{
				create(key->to());
			}
			send_key = false;
		}
	}
	if (send_key)
	{
		if (evt.state() == TouchEvent::TOUCH_DOWN)
		{
			UIManager::notifier()->push_event(new KeyEvent(widget->id(), KeyEvent::KEY_DOWN));
		}
		else if (evt.state() == TouchEvent::TOUCH_UP)
		{
			UIManager::notifier()->push_event(new KeyEvent(widget->id(), KeyEvent::KEY_UP));
		}
	}
}

