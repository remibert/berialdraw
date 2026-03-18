#include "berialdraw_imp.hpp"
using namespace berialdraw;

WidgetStyle::WidgetStyle()
{
	m_extend      = 0;
	m_size_policy = 0;
	m_id          = 0;
	m_pressed     = 0;
	m_focusable   = 0;
	m_focused     = 0;
	m_checked     = 0;
	m_pressable   = 0;
	m_flow_place  = 0;
	m_selectable  = 0;
	m_flow        = 0;
	m_flow_in_children = 0;
}

/** Serialize the content of widget into json */
void WidgetStyle::serialize(JsonIterator & it)
{
	m_min_size.serialize(StyleNames::WIDGET_MIN_SIZE,it),
	m_max_size.serialize(StyleNames::WIDGET_MAX_SIZE,it);
	it[StyleNames::KEY_ID]                  = (int)m_id       ;
	it[StyleNames::WIDGET_CELL][StyleNames::WIDGET_ROW]         = (int)m_row      ;
	it[StyleNames::WIDGET_CELL][StyleNames::WIDGET_COLUMN]      = (int)m_column   ;
	berialdraw::serialize(it, (Extend)m_extend);
	berialdraw::serialize(it, (SizePolicy)m_size_policy);
	it[StyleNames::WIDGET_PRESSED]       = (int)m_pressed;
	it[StyleNames::WIDGET_CHECKED]       = (int)m_checked;
	it[StyleNames::WIDGET_FOCUSABLE]     = (int)m_focusable;
	it[StyleNames::WIDGET_SELECTABLE]    = (int)m_selectable;
	it[StyleNames::WIDGET_PRESSABLE]     = (int)m_pressable;
	it[StyleNames::WIDGET_FLOW]    = (int)m_flow;
}

/** Unserialize the content of widget from json */
void WidgetStyle::unserialize(JsonIterator & it)
{
	m_min_size.unserialize(StyleNames::WIDGET_MIN_SIZE,it),
	m_max_size.unserialize(StyleNames::WIDGET_MAX_SIZE,it);
	m_id     = it[StyleNames::KEY_ID]             | (int)m_id;
	m_row    = it[StyleNames::WIDGET_CELL][StyleNames::WIDGET_ROW]    | (int)m_row;
	m_column = it[StyleNames::WIDGET_CELL][StyleNames::WIDGET_COLUMN] | (int)m_column;
	Extend extend = (Extend)m_extend;
	berialdraw::unserialize(it, extend);
	m_extend = extend;

	SizePolicy size_policy = (SizePolicy)m_size_policy;
	berialdraw::unserialize(it, size_policy);
	m_size_policy = size_policy;
	m_pressed   = (int)it[StyleNames::WIDGET_PRESSED]  | m_pressed;
	m_checked   = (int)it[StyleNames::WIDGET_CHECKED]  | m_checked;
	m_focusable = (int)it[StyleNames::WIDGET_FOCUSABLE] | m_focusable;
	m_selectable = (int)it[StyleNames::WIDGET_SELECTABLE] | m_selectable;
	m_pressable  = (int)it[StyleNames::WIDGET_PRESSABLE] | m_pressable;
	m_flow       = (int)it[StyleNames::WIDGET_FLOW] | m_flow;
}

/** Copy operator */
WidgetStyle& WidgetStyle::operator=(const WidgetStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void WidgetStyle::set(const WidgetStyle & other)
{
	if (this != &other)
	{
		m_min_size        = other.m_min_size,
		m_max_size        = other.m_max_size;
		m_id              = other.m_id;
		m_row             = other.m_row;
		m_column          = other.m_column;
		m_extend          = other.m_extend;
		m_size_policy     = other.m_size_policy;
		m_focusable       = other.m_focusable;
		m_checked         = other.m_checked;
		m_selectable      = other.m_selectable;
		m_pressable       = other.m_pressable;
		m_flow_place      = other.m_flow_place;
		m_flow            = other.m_flow;
		m_flow_in_children = other.m_flow_in_children;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * WidgetStyle::create()
{
	return new WidgetStyle;
}

/** Set the row in the grid */
void WidgetStyle::row(uint16_t r)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_row = r;
}

/** Set the column in the grid */
void WidgetStyle::column(uint16_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_column = c;
}

/** Set the cell in the grid */
void WidgetStyle::cell(uint16_t r, uint16_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_row = r;
	m_column = c;
}

/** Set the cell size policy */
void WidgetStyle::size_policy(SizePolicy v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_size_policy = (SizePolicy)v;
}

/** Set the minsize */
void WidgetStyle::min_size(const Size & min_size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_min_size = min_size_;
}

/** Set the minsize with width and height in pixels */
void WidgetStyle::min_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_min_size.set(w,h);
}

/** Set the minsize with a precision of 64th of a pixel */
void WidgetStyle::min_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_min_size.set_(w,h);
}

/** Set the max size */
void WidgetStyle::max_size(const Size & max_size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_max_size = max_size_;
}

/** Set the max size with width and height */
void WidgetStyle::max_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_max_size.set(w,h);
}

/** Set the max size with a precision of 64th of a pixel */
void WidgetStyle::max_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_max_size.set_(w,h);
}

/** Set the id */
void WidgetStyle::id(uint16_t v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_id = (uint16_t)v;
}

/** Set the extend */
void WidgetStyle::extend(Extend v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_extend = (Extend)v;
}

/** Set the pressable */
void WidgetStyle::pressable(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_pressable = v;
}

/** Set the pressed */
void WidgetStyle::pressed(bool v)
{
	if (m_pressable)
	{
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
		m_pressed = v;
	}
}

/** Set the focusable */
void WidgetStyle::focusable(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focusable = v;
}

/** Set the selectable (used for the SelectEvent) */
void WidgetStyle::selectable(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_selectable = v;
}

/** Set the checked */
void WidgetStyle::checked(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_checked = v;
}

/** Set the focused */
void WidgetStyle::focused(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_focused = v;
}

#ifdef _DEBUG
void WidgetStyle::test1()
{
	WidgetStyle prop;
	String buffer;
	Json json;
	JsonIterator win(json);
	prop.serialize(win);
	json.serialize(buffer,1);

	json.unserialize(buffer.c_str());
}

void WidgetStyle::test2()
{
	uint32_t s;
	s = sizeof(Margin);
	s = sizeof(String);
	s = sizeof(WidgetStyle);
	s = sizeof(BorderStyle);
}

void WidgetStyle::test3()
{
}

void WidgetStyle::test4()
{
}

void WidgetStyle::test()
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
