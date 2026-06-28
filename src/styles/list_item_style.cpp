#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ListItemStyle::ListItemStyle()
{
	m_leading_modified  = 1;
	m_trailing_modified = 1;
}

/** Serialize the content of widget into json */
void ListItemStyle::serialize(JsonIterator& it)
{
	it[StyleNames::LIST_ITEM_LEADING]  = m_leading;
	it[StyleNames::LIST_ITEM_TRAILING] = m_trailing;
}

/** Unserialize the content of widget from json */
void ListItemStyle::unserialize(JsonIterator& it)
{
	m_leading = it[StyleNames::LIST_ITEM_LEADING] | m_leading;
	m_leading_modified = 1;
	m_trailing = it[StyleNames::LIST_ITEM_TRAILING] | m_trailing;
	m_trailing_modified = 1;
}

/** Copy operator */
ListItemStyle& ListItemStyle::operator=(const ListItemStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void ListItemStyle::set(const ListItemStyle& other)
{
	if (this != &other)
	{
		m_leading = other.m_leading;
		m_leading_modified = 1;
		m_trailing = other.m_trailing;
		m_trailing_modified = 1;
	}
}

Style* ListItemStyle::create()
{
	return new ListItemStyle;
}

/** Get the leading value */
const String& ListItemStyle::leading()
{
	return m_leading;
}

/** Set leading value with printf format */
void ListItemStyle::leading(const char* leading_, ...)
{
	va_list args;
	va_start(args, leading_);
	m_leading_modified = 1;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_leading.vprint(leading_, args);
	va_end(args);
}

/** Set leading value with string */
void ListItemStyle::leading(const String& str)
{
	m_leading_modified = 1;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_leading = str;
}

/** Set leading value with unicode value */
void ListItemStyle::leading(wchar_t value)
{
	m_leading_modified = 1;
	m_leading = value;
}

/** Get the trailing value */
const String& ListItemStyle::trailing()
{
	return m_trailing;
}

/** Set trailing value with printf format */
void ListItemStyle::trailing(const char* trailing_, ...)
{
	va_list args;
	va_start(args, trailing_);
	m_trailing_modified = 1;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_trailing.vprint(trailing_, args);
	va_end(args);
}

/** Set trailing value with string */
void ListItemStyle::trailing(const String& str)
{
	m_trailing_modified = 1;
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_trailing = str;
}

/** Set trailing value with unicode value */
void ListItemStyle::trailing(wchar_t value)
{
	m_trailing_modified = 1;
	m_trailing = value;
}
