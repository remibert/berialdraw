#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget */
Styles::Styles()
{
	m_style = "pearl";
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
	for(int index = 0; index < (int)m_names.size(); index++)
	{
		delete m_names[index];
	}
	m_names.clear();
	for(int index = 0; index < (int)m_styles.size(); index++)
	{
		delete m_styles[index];
	}
	m_styles.clear();
	delete m_json;
	m_json = 0;
	m_json_name = "";
}


/** Get properties */
bool Styles::apply(const char * classname, WidgetStyle * properties)
{
	WidgetStyle *style = dynamic_cast<WidgetStyle*>(select(classname, "widget", WidgetStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, CommonStyle * properties)
{
	CommonStyle *style = dynamic_cast<CommonStyle*>(select(classname, "common", CommonStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, TextStyle * properties)
{
	TextStyle *style = dynamic_cast<TextStyle*>(select(classname, "text", TextStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, EditStyle * properties)
{
	EditStyle *style = dynamic_cast<EditStyle*>(select(classname, "edit", EditStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, SwitchStyle * properties)
{
	SwitchStyle *style = dynamic_cast<SwitchStyle*>(select(classname, "switch", SwitchStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, SliderStyle * properties)
{
	SliderStyle *style = dynamic_cast<SliderStyle*>(select(classname, "slider", SliderStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, ProgressBarStyle * properties)
{
	ProgressBarStyle *style = dynamic_cast<ProgressBarStyle*>(select(classname, "progressbar", ProgressBarStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Get properties */
bool Styles::apply(const char * classname, BorderStyle * properties)
{
	BorderStyle *style = dynamic_cast<BorderStyle*>(select(classname, "border", BorderStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Apply icon properties */
bool Styles::apply(const char * classname, IconStyle * properties)
{
	IconStyle *style = dynamic_cast<IconStyle*>(select(classname, "icon", IconStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
}

/** Apply scrollview properties */
bool Styles::apply(const char * classname, ScrollViewStyle * properties)
{
	ScrollViewStyle *style = dynamic_cast<ScrollViewStyle*>(select(classname, "scrollview", ScrollViewStyle::create));
	if (properties && style)
	{
		*properties = *style;
	}
	return true;
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
Style * Styles::select(const char * classname, const char * properties, StyleCreator_t creator)
{
	Style * result = 0;
	String style_name;
	style_name.print("%s.%s",classname, properties);
	for (int index = 0; index < (int)m_names.size(); index++)
	{
		if (*m_names[index] == style_name)
		{
			result = m_styles[index];
			break;
		}
	}
	if (result == 0)
	{
		result = load(classname, properties, creator);
	}
	return result;
}

/** Get style filename according to class name */
void Styles::filename(const char * classname, String & filename_)
{
	filename_.print("${ui.styles}/%s/%s.json",m_style.c_str(),classname);
}

/** Load the style according to the name specified */
Style * Styles::load(const char * classname, const char * properties, StyleCreator_t creator)
{
	Style * result = 0;
	File file;
	String filename_;
	
	filename(classname, filename_);

	if (m_json_name != filename_)
	{
		if (m_json)
		{
			delete m_json;
			m_json = 0;
		}
		m_json_name = "";

		if (file.open(filename_,"rb") != -1)
		{
			m_json = new Json;
			try
			{
				m_json->unserialize(file);
				m_json_name = filename_;
			}
			catch(...)
			{
				delete m_json;
				m_json = 0;
			}
		}
	}
	if (m_json && m_json_name == filename_)
	{
		JsonIterator it(*m_json);

		result = creator();
		if (result)
		{
			uint32_t size = sizeof(*result);
			result->unserialize(it);
			String * style_name = new String;
			if (style_name)
			{
				style_name->print("%s.%s",classname, properties);
				m_names.push_back(style_name);
				m_styles.push_back(result);
			}
			else
			{
				delete result;
				result = 0;
			}
		}
	}
	return result;
}

#ifdef _DEBUG
void Styles::test1()
{
}

void Styles::test2()
{
}

void Styles::test3()
{
}

void Styles::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test3();
		test2();
		test1();
	}
}
#endif
