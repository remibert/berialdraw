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
	m_focus_thickness = 0;
}

/** Serialize the content of widget into json */
void WidgetStyle::serialize(JsonIterator & it)
{
	m_min_size.serialize("min-size",it),
	m_max_size.serialize("max-size",it);
	it["id"]                  = (int)m_id       ;
	it["cell"]["row"]         = (int)m_row      ;
	it["cell"]["column"]      = (int)m_column   ;
	berialdraw::serialize(it, (Extend)m_extend);
	berialdraw::serialize(it, (SizePolicy)m_size_policy);
	it["pressed"]       = (int)m_pressed;
	it["checked"]       = (int)m_checked;
	it["focusable"]     = (int)m_focusable;
	it["selectable"]    = (int)m_selectable;
	it["pressable"]     = (int)m_pressable;
	it["flow"]    = (int)m_flow;
	it["focus-thickness"] = (int)m_focus_thickness;
}

/** Unserialize the content of widget from json */
void WidgetStyle::unserialize(JsonIterator & it)
{
	m_min_size.unserialize("min-size",it),
	m_max_size.unserialize("max-size",it);
	m_id     = it["id"]             | (int)m_id;
	m_row    = it["cell"]["row"]    | (int)m_row;
	m_column = it["cell"]["column"] | (int)m_column;
	Extend extend = (Extend)m_extend;
	berialdraw::unserialize(it, extend);
	m_extend = extend;

	SizePolicy size_policy = (SizePolicy)m_size_policy;
	berialdraw::unserialize(it, size_policy);
	m_size_policy = size_policy;
	m_pressed   = (int)it["pressed"]  | m_pressed;
	m_checked   = (int)it["checked"]  | m_checked;
	m_focusable = (int)it["focusable"] | m_focusable;
	m_selectable = (int)it["selectable"] | m_selectable;
	m_pressable  = (int)it["pressable"] | m_pressable;
	m_flow       = (int)it["flow"] | m_flow;
	m_focus_thickness    = (int)it["focus-thickness"] | m_focus_thickness;
}

/** Set properties with another */
void WidgetStyle::set(const WidgetStyle & other)
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
	m_focus_thickness = other.m_focus_thickness;
	m_flow_in_children = other.m_flow_in_children;
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
}

Style * WidgetStyle::create()
{
	return new WidgetStyle;
}

/** Get the row in the grid */
uint16_t WidgetStyle::row() const
{
	return m_row;
}

/** Get the column in the grid */
uint16_t WidgetStyle::column() const
{
	return m_column;
}

/** Set the row in the grid */
void WidgetStyle::row(uint16_t r)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_row = r;
}

/** Set the column in the grid */
void WidgetStyle::column(uint16_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_column = c;
}

/** Set the cell in the grid */
void WidgetStyle::cell(uint16_t r, uint16_t c)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_row = r;
	m_column = c;
}

/** Get the cell size policy */
SizePolicy WidgetStyle::size_policy() const
{
	return (SizePolicy)m_size_policy;
}

/** Set the cell size policy */
void WidgetStyle::size_policy(SizePolicy v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_size_policy = (SizePolicy)v;
}

/** Get the minsize */
const Size & WidgetStyle::min_size() const
{
	return m_min_size;
}

/** Set the minsize */
void WidgetStyle::min_size(const Size & min_size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_min_size = min_size_;
}

/** Set the minsize with width and height in pixels */
void WidgetStyle::min_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_min_size.set(w,h);
}

/** Set the minsize with a precision of 64th of a pixel */
void WidgetStyle::min_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_min_size.set_(w,h);
}

/** Get the max size */
const Size & WidgetStyle::max_size() const
{
	return m_max_size;
}

/** Set the max size */
void WidgetStyle::max_size(const Size & max_size_)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_max_size = max_size_;
}

/** Set the max size with width and height */
void WidgetStyle::max_size(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_max_size.set(w,h);
}

/** Set the max size with a precision of 64th of a pixel */
void WidgetStyle::max_size_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_max_size.set_(w,h);
}

/** Get the id */
uint16_t WidgetStyle::id() const
{
	return (uint16_t)m_id;
}

/** Set the id */
void WidgetStyle::id(uint16_t v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_id = (uint16_t)v;
}

/** Get the extend */
Extend WidgetStyle::extend() const
{
	return (Extend)m_extend;
}

/** Set the extend */
void WidgetStyle::extend(Extend v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_extend = (Extend)v;
}

/** Get the pressable */
bool WidgetStyle::pressable() const
{
	return m_pressable;
}

/** Set the pressable */
void WidgetStyle::pressable(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
	m_pressable = v;
}

/** Get the pressed */
bool WidgetStyle::pressed() const
{
	return m_pressed;
}

/** Set the pressed */
void WidgetStyle::pressed(bool v)
{
	if (m_pressable)
	{
		UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
		m_pressed = v;
	}
}


/** Get the focusable */
bool WidgetStyle::focusable() const
{
	return m_focusable;
}

/** Set the focusable */
void WidgetStyle::focusable(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
	m_focusable = v;
}


/** Get the selectable (used for the SelectEvent) */
bool WidgetStyle::selectable() const
{
	return m_selectable;
}

/** Set the selectable (used for the SelectEvent) */
void WidgetStyle::selectable(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
	m_selectable = v;
}

/** Get the checked */
bool WidgetStyle::checked() const
{
	return m_checked;
}

/** Set the checked */
void WidgetStyle::checked(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
	m_checked = v;
}

/** Get the focused */
bool WidgetStyle::focused() const
{
	return m_focused;
}

/** Set the focused */
void WidgetStyle::focused(bool v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
	m_focused = v;
}



/** Get the focus thickness (16 pixels max)*/
Dim WidgetStyle::focus_thickness() const
{
	return m_focus_thickness;
}

/** Set the focus thickness in pixels (16 pixels max)*/
void WidgetStyle::focus_thickness(Dim v)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPAINT);
	m_focus_thickness = v;
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
	test4();
	test3();
	test2();
	test1();
}
#endif
