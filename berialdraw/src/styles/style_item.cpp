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

/** Constructor with name and compiled Style */
StyleItem::StyleItem(const String& name, Style* style)
	: m_name(name), m_properties("{}"), m_style(style)
{
}

/** Copy constructor */
StyleItem::StyleItem(const StyleItem& other)
	: m_name(other.m_name), m_properties(other.m_properties), m_style(other.m_style)
{
}

/** Move constructor */
StyleItem::StyleItem(StyleItem&& other) noexcept
	: m_name(std::move(other.m_name)), 
	  m_properties(std::move(other.m_properties)),
	  m_style(other.m_style)
{
	other.m_style = nullptr;
}

/** Destructor */
StyleItem::~StyleItem()
{
	delete m_style;
}

/** Get the properties as a Json (returns by value with move semantics) */
Json StyleItem::properties() const
{
	Json json;
	if (m_properties.size())
	{
		json.unserialize(m_properties);
	}
	return json; 
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

