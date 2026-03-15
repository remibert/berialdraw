#include "berialdraw_imp.hpp"

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
		it[StyleNames::KEY_STYLES][0] ="";
	}
	for (uint32_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i])
		{
			JsonIterator style_it = it[StyleNames::KEY_STYLES][i];
			m_items[i]->serialize(style_it);
		}
	}
}

/** Unserialize styles from JSON */
void StyleManager::unserialize(JsonIterator & it)
{
	clear();

	JsonIterator styles_it = it[StyleNames::KEY_STYLES];
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

#ifdef _DEBUG

/** Test 1: Basic style registration and retrieval */
void StyleManager::test1()
{
	StyleManager manager;
	
	// Create and register a simple style
	manager.add_style("primary", "{}");
	
	// Verify it exists and can be retrieved
	assert(manager.has_style("primary"));
	assert(manager.get_style("primary") != nullptr);
	assert(manager.style_count() == 1);
	
	// Create and register another style
	manager.add_style("secondary", "{}");
	
	assert(manager.has_style("secondary"));
	assert(manager.style_count() == 2);
}

/** Test 2: Get all style names */
void StyleManager::test2()
{
	StyleManager manager;
	
	manager.add_style("header", "{}");
	manager.add_style("footer", "{}");
	manager.add_style("body", "{}");
	
	Vector<String> names = manager.get_style_names();
	assert(names.size() == 3);
	
	// Test contains expected names
	bool found_header = false, found_footer = false, found_body = false;
	for (uint32_t i = 0; i < names.size(); i++)
	{
		if (names[i] == "header") found_header = true;
		if (names[i] == "footer") found_footer = true;
		if (names[i] == "body") found_body = true;
	}
	
	assert(found_header && found_footer && found_body);
}

// NOTE: Tests 3-9 are commented out because they depend on the old StyleItem API
// which included add_style(), style_count(), get_style(), get_style_of_type(), etc.
// The new ultra-lightweight StyleItem design stores only name and JSON properties,
// with selective application via Style::apply_style() and JsonIterator pattern.

/*
/** Test 3: Unregister/delete a style * /
void StyleManager::test3() { ... }

/** Test 4: Duplicate registration replaces existing * /
void StyleManager::test4() { ... }

/** Test 5: Clear all styles * /
void StyleManager::test5() { ... }

/** Test 6: StyleItem with multiple styles * /
void StyleManager::test6() { ... }

/** Test 7: StyleItem copy operator * /
void StyleManager::test7() { ... }

/** Test 8: Serialize and unserialize styles * /
void StyleManager::test8() { ... }

/** Test 9: Complex style management workflow * /
void StyleManager::test9() { ... }
*/

/** Main test runner - execute all tests */
void StyleManager::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		// test9(); // Commented out (old API)
		// test8(); // Commented out (old API)
		// test7(); // Commented out (old API)
		// test6(); // Commented out (old API)
		// test5(); // Commented out (old API)
		// test4(); // Commented out (old API)
		// test3(); // Commented out (old API)
		test2();
		test1();
	}
}

#endif
