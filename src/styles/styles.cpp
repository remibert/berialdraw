#include "berialdraw_imp.hpp"

using namespace berialdraw;

// ============================================================================
// JsonFileCache Implementation
// ============================================================================

/** Create JsonFileCache */
JsonFileCache::JsonFileCache()
	: m_json(0)
{
}

/** Destroy JsonFileCache */
JsonFileCache::~JsonFileCache()
{
	clear();
}

/** Load JSON from file if not already loaded, or return existing cache if same filename */
std::unique_ptr<JsonIterator> JsonFileCache::get_iterator(const String & filename)
{
	File file;

	// If different file, clear previous cache
	if (m_filename != filename)
	{
		clear();
		m_filename = filename;

		// Try to open and load the new file
		if (file.open(filename.c_str(), "rb") != -1)
		{
			m_json = new Json;
			try
			{
				m_json->unserialize(file);
			}
			catch (...)
			{
				delete m_json;
				m_json = 0;
				m_filename = "";
			}
		}
	}

	// Return iterator if JSON is loaded
	if (m_json)
	{
		return std::make_unique<JsonIterator>(*m_json);
	}

	return nullptr;
}

/** Clear the cache and free resources */
void JsonFileCache::clear()
{
	if (m_json)
	{
		delete m_json;
		m_json = 0;
	}
	m_filename = "";
}

// ============================================================================
// Styles Implementation
// ============================================================================

/** Create widget */
Styles::Styles()
{
	m_style = "pearl";
	add_style("undefined","{}");
}

/** Destroy widget */
Styles::~Styles()
{
	clear();
}

/** Get style name */
const String & Styles::style()
{
	return m_style;
}

/** Set style name */
void Styles::style(const String & name)
{
	if (m_style != name)
	{
		clear();
	}

	m_style = name;
}

/** Clear styles */
void Styles::clear()
{
	for (int i = 0; i < (int)m_items.size(); i++)
	{
		StyleItem* item = m_items[i];
		delete item;
	}
	m_items.clear();
	m_json_cache.clear();
}

/** Find style item index by name */
uint32_t Styles::find_index(const String& name) const
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

/** Add a style with a name and properties (creates new StyleItem) */
void Styles::add_style(const String& name, const String& properties)
{
	// Check if already exists and remove it
	remove_style(name);
	
	// Create and add new StyleItem
	StyleItem* item = new StyleItem(name, properties);
	m_items.push_back(item);
}


/** Add a style with a name and properties (creates new StyleItem)
@param iter json iterator */
void Styles::add_style(JsonIterator & iter)
{
	// If generic styles is defined in the widget
	if (iter.exists(StyleNames::STYLE_STYLES))
	{
		if (iter[StyleNames::STYLE_STYLES].type() == JsonType::ARRAY)
		{
			JsonIterator array_json = iter[StyleNames::STYLE_STYLES];
			for(array_json.first(); array_json.exist(); array_json.next())
			{
				StyleItem * item = new StyleItem;
				JsonIterator js_item(array_json.child());
				item->unserialize(js_item);
				remove_style(js_item[StyleNames::STYLE_NAME]);
				m_items.push_back(item);
			}
		}
		else if (iter[StyleNames::STYLE_STYLES].type() == JsonType::OBJECT)
		{
			StyleItem * item = new StyleItem;
			JsonIterator js_item = iter[StyleNames::STYLE_STYLES];
			item->unserialize(js_item);
			remove_style(js_item[StyleNames::STYLE_NAME]);
			m_items.push_back(item);
		}
	}
}



/** Remove a style by name */
bool Styles::remove_style(const String& name)
{
	uint32_t index = find_index(name);
	if (index != ~0)
	{
		m_items.remove(index);
		return true;
	}
	return false;
}

/** Get a registered style by name */
StyleItem* Styles::get_style(const String& name) const
{
	uint32_t index = find_index(name);
	if (index == ~0)
	{
		index = 0;
	}
	return m_items[index];
}

/** Check if a style exists */
bool Styles::has_style(const String& name) const
{
	return find_index(name) != ~0;
}

/** Get mappings properties */
const Mappings * Styles::mappings(const char * classname)
{
	const Mappings *result = dynamic_cast<Mappings*>(select(classname, "mappings", Mappings::create));
	return result;
}

/** Get keys properties */
const Keys * Styles::keys(const char * classname)
{
	const Keys *result = dynamic_cast<Keys*>(select(classname, "keys", Keys::create));
	return result;
}

/** Select the style according to the name specified, load it if not yet existing */
Style * Styles::select(const char * classname, const char * property_name, StyleCreator_t creator)
{
	Style * result = nullptr;
	String style_name;
	style_name.print("widget.%s.%s", classname, property_name);
	for (int index = 0; index < (int)m_items.size(); index++)
	{
		if (m_items[index]->name() == style_name)
		{
			result = m_items[index]->style();
			break;
		}
	}
	if (result == nullptr)
	{
		result = load(classname, property_name, creator);
	}
	return result;
}

/** Load the style according to the name specified */
Style * Styles::load(const char * classname, const char * property_name, StyleCreator_t creator)
{
	Style * result = nullptr;
	String filename_;
	filename_.print("$(ui.styles)/%s/%s.json", m_style.c_str(), classname);

	// Get iterator from cache (handles lazy loading and smart reuse)
	auto it = m_json_cache.get_iterator(filename_);

	if (it)
	{
		result = creator();
		if (result)
		{
			result->unserialize(*it);
			String style_name;
			style_name.print("widget.%s.%s", classname, property_name);
			m_items.push_back(new StyleItem(style_name, result));
		}
	}
	return result;
}

// ============================================================================
// Template Implementations
// ============================================================================

template<typename T>
bool Styles::apply(Widget* widget, T* properties)
{
	if (!widget || !properties) return false;
	return apply(widget->classname(), properties);
}

// Explicit template instantiations for all Style types
template bool Styles::apply<CommonStyle>(Widget* widget, CommonStyle* properties);
template bool Styles::apply<WidgetStyle>(Widget* widget, WidgetStyle* properties);
template bool Styles::apply<BorderStyle>(Widget* widget, BorderStyle* properties);
template bool Styles::apply<TextStyle>(Widget* widget, TextStyle* properties);
template bool Styles::apply<EditStyle>(Widget* widget, EditStyle* properties);
template bool Styles::apply<SwitchStyle>(Widget* widget, SwitchStyle* properties);
template bool Styles::apply<CheckboxStyle>(Widget* widget, CheckboxStyle* properties);
template bool Styles::apply<RadioStyle>(Widget* widget, RadioStyle* properties);
template bool Styles::apply<SliderStyle>(Widget* widget, SliderStyle* properties);
template bool Styles::apply<ProgressBarStyle>(Widget* widget, ProgressBarStyle* properties);
template bool Styles::apply<IconStyle>(Widget* widget, IconStyle* properties);
template bool Styles::apply<ScrollViewStyle>(Widget* widget, ScrollViewStyle* properties);
template bool Styles::apply<TableViewStyle>(Widget* widget, TableViewStyle* properties);
template bool Styles::apply<Mappings>(Widget* widget, Mappings* properties);
template bool Styles::apply<Keys>(Widget* widget, Keys* properties);
template bool Styles::apply<PieStyle>(Widget* widget, PieStyle* properties);
template bool Styles::apply<LineStyle>(Widget* widget, LineStyle* properties);
template bool Styles::apply<RoundStyle>(Widget* widget, RoundStyle* properties);

/** Specialization for Key to resolve ambiguous 'create()' from multiple inheritance */
template<>
bool Styles::apply<Key>(Widget* widget, Key* properties)
{
	if (!widget || !properties) return false;
	return apply(widget->classname(), properties);
}


