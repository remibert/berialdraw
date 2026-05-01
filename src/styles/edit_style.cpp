#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
EditStyle::EditStyle()
{
}

/** Destructor */
EditStyle::~EditStyle()
{
	delete m_place_holder;
}

/** Serialize the content of widget into json */
void EditStyle::serialize(JsonIterator & it)
{
	it[StyleNames::EDIT_MAX_LINES]      = m_max_lines;
	it[StyleNames::EDIT_MAX_COLUMNS]    = m_max_columns;
	it[StyleNames::EDIT_SELECT_COLOR]   = m_select_color;
	it[StyleNames::EDIT_CURSOR_COLOR]   = m_cursor_color;
	it[StyleNames::EDIT_PLACEHOLDER_COLOR]   = m_place_holder_color;
	it[StyleNames::EDIT_PASSWORD]      = m_password;
	if (m_place_holder)
	{
		it[StyleNames::EDIT_PLACEHOLDER] = *m_place_holder;
	}
}

/** Unserialize the content of widget from json */
void EditStyle::unserialize(JsonIterator & it)
{
	m_max_lines    = (int)(it[StyleNames::EDIT_MAX_LINES]      | (int)m_max_lines);
	m_max_columns  = (int)(it[StyleNames::EDIT_MAX_COLUMNS]    | (int)m_max_columns);
	m_select_color = (int)(it[StyleNames::EDIT_SELECT_COLOR]  | (int)m_select_color);
	m_cursor_color = (int)(it[StyleNames::EDIT_CURSOR_COLOR]  | (int)m_cursor_color);
	m_place_holder_color = (int)(it[StyleNames::EDIT_PLACEHOLDER_COLOR]  | (int)m_place_holder_color);
	m_password    = (bool)(it[StyleNames::EDIT_PASSWORD]     | (bool)m_password);

	delete m_place_holder;
	if (it[StyleNames::EDIT_PLACEHOLDER].exist())
	{
		m_place_holder = new String(it[StyleNames::EDIT_PLACEHOLDER]);
	}
	else
	{
		m_place_holder = 0;
	}
}

/** Copy operator */
EditStyle& EditStyle::operator=(const EditStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void EditStyle::set(const EditStyle & other)
{
	if (this != &other)
	{
		m_max_lines      = other.m_max_lines;
		m_max_columns    = other.m_max_columns;
		m_select_color  = other.m_select_color;
		m_cursor_color  = other.m_cursor_color;
		m_password      = other.m_password;

		if (other.m_place_holder)
		{
			m_place_holder = new String(*m_place_holder);
		}
		else
		{
			m_place_holder = 0;
		}
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	}
}

Style * EditStyle::create()
{
	return new EditStyle;
}

/** Set the max visible lines in the edit */
void EditStyle::max_lines(uint16_t v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_max_lines = v;
}

/** Set the max visible column in the edit */
void EditStyle::max_columns(uint16_t v)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_max_columns = v;
}

/** Set the selection color */
void EditStyle::select_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_select_color = col;
}

/** Set the selection color with alpha */
void EditStyle::select_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_select_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Set the cursor color */
void EditStyle::cursor_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_cursor_color = col;
}

/** Set the cursor color with alpha */
void EditStyle::cursor_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_cursor_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Hides characters for password entry */
void EditStyle::password(bool state)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_password = state;
}

/** Get the place holder value */
const String & EditStyle::place_holder()
{
	return *m_place_holder;
}

/** Set place holder value with printf format */
void EditStyle::place_holder(const char * place_holder_, ...)
{
	va_list args;
	va_start(args, place_holder_);
	if (m_place_holder == 0)
	{
		m_place_holder = new String("");
	}
	
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_place_holder->vprint(place_holder_, args);
	va_end(args);
}

/** Set place holder value with string */
void EditStyle::place_holder(const String & str)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	if (m_place_holder == 0)
	{
		m_place_holder = new String("");
	}
	*m_place_holder = str;
}


/** Set the place holder color */
void EditStyle::place_holder_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_place_holder_color = col;
}

/** Set the place holder color with alpha */
void EditStyle::place_holder_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	m_place_holder_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}
