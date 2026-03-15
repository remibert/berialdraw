#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
StyleItem::StyleItem()
{
}

/** Constructor with name */
StyleItem::StyleItem(const String& name)
	: m_name(name), m_properties("{}")
{
}

/** Constructor with name and properties */
StyleItem::StyleItem(const String& name, const String& json_properties)
	: m_name(name), m_properties(json_properties)
{
}

/** Destructor */
StyleItem::~StyleItem()
{
	// Nothing to clean - no dynamic allocations
}

/** Get the properties as a JsonIterator 
Parses the JSON string and returns iterator for selective unserialize */
JsonIterator StyleItem::properties() const
{
	// Parse the JSON properties string using the Json class
	Json json(m_properties.c_str());
	return JsonIterator(json);
}

/** Serialize the StyleItem to JSON */
void StyleItem::serialize(JsonIterator & it)
{
	it[StyleNames::STYLE_NAME] = m_name;
	it[StyleNames::STYLE_PROPERTIES] = m_properties;
}

/** Unserialize the StyleItem from JSON */
void StyleItem::unserialize(JsonIterator & it)
{
	m_name = (const char*)(it[StyleNames::STYLE_NAME] | "");
	m_properties = (const char*)(it[StyleNames::STYLE_PROPERTIES] | "{}");
}

