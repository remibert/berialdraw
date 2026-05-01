#include "berialdraw_imp.hpp"
#include "styles/style_manager.hpp"

using namespace berialdraw;

/** Constructor */
StyleManager::StyleManager()
{
}

/** Destructor */
StyleManager::~StyleManager()
{
	clear();
}

/** Add a style with a name and properties (creates new StyleItem) */
void StyleManager::add_style(const String& name, const String& properties)
{
	// Check if already exists and remove it
	remove_style(name);
	
	// Create and add new StyleItem
	StyleItem* item = new StyleItem(name, properties);
	m_items.push_back(item);
}

/** Remove a style by name */
bool StyleManager::remove_style(const String& name)
{
	uint32_t index = find_index(name);
	if (index != ~0)
	{
		delete m_items[index];
		// Remove from vector by shifting elements
		for (uint32_t i = index; i < m_items.size() - 1; i++)
		{
			m_items[i] = m_items[i + 1];
		}
		// Reduce size
		m_items.resize(m_items.size() - 1);
		return true;
	}
	return false;
}

/** Get a registered style by name */
StyleItem* StyleManager::get_style(const String& name) const
{
	uint32_t index = find_index(name);
	if (index != ~0)
	{
		return m_items[index];
	}
	return nullptr;
}

/** Check if a style exists */
bool StyleManager::has_style(const String& name) const
{
	return find_index(name) != ~0;
}

/** Get all registered style names */
Vector<String> StyleManager::get_style_names() const
{
	Vector<String> names;
	for (uint32_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i])
		{
			names.push_back(m_items[i]->name());
		}
	}
	return names;
}

/** Clear all registered styles */
void StyleManager::clear()
{
	for (uint32_t i = 0; i < m_items.size(); i++)
	{
		delete m_items[i];
	}
	m_items.clear();
}

/** Serialize all styles to JSON */
void StyleManager::serialize(JsonIterator & it) const
{
	if (m_items.size() > 0)
	{
		it[StyleNames::STYLE_STYLES][0] ="";
	}
	for (uint32_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i])
		{
			JsonIterator style_it = it[StyleNames::STYLE_STYLES][i];
			m_items[i]->serialize(style_it);
		}
	}
}

/** Unserialize styles from JSON */
void StyleManager::unserialize(JsonIterator & it)
{
	clear();

	JsonIterator styles_it = it[StyleNames::STYLE_STYLES];
	for (styles_it.first(); styles_it.exist(); styles_it.next())
	{
		StyleItem* item = new StyleItem();
		JsonIterator child_it = styles_it.child();
		item->unserialize(child_it);
		m_items.push_back(item);
	}
}

/** Find style item index by name */
uint32_t StyleManager::find_index(const String& name) const
{
	for (uint32_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i] && m_items[i]->name() == name)
		{
			return i;
		}
	}
	return ~0;
}

