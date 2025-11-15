#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create colors */
Colors::Colors()
{
	m_theme = Color::THEME_LIME;
	appearance(0);
}

/** Destroy widget */
Colors::~Colors()
{
	m_colors_primary.clear();
}

/** Choose the color theme
@param color theme */
void Colors::theme(uint32_t color_theme)
{
	if (color_theme >= Color::FIRST_THEME_COLOR && color_theme <= Color::LAST_THEME_COLOR)
	{
		uint32_t col;
		m_theme = color_theme;
		col = color(color_theme);

		m_theme_values [Color::THEME_BACK_COLOR             - Color::FIRST_THEME_VALUE] =  col;

		for (int i = THEME_BACK_COLOR_LIGHT_1; i <= THEME_BACK_COLOR_LIGHT_15; i++)
		{
			m_theme_values [i-FIRST_THEME_VALUE] = Hsl::add_color(col, Color::WHITE, ((i-THEME_BACK_COLOR_LIGHT_1) * 17) +8);
		}

		for (int i = THEME_BACK_COLOR_DARK_1; i <= THEME_BACK_COLOR_DARK_15; i++)
		{
			m_theme_values [i-FIRST_THEME_VALUE] = Hsl::add_color(col, Color::BLACK, (i-THEME_BACK_COLOR_DARK_1) * 17);
		}

		if (color_theme < m_colors_secondary.size())
		{
			m_theme_values [Color::THEME_FORE_COLOR  - Color::FIRST_THEME_VALUE] = m_colors_secondary[color_theme];
		}
	}
}

/** Choose the name of the appearance mode
@param name name of appearance (can be "light", "dark")
@return true if load with success */
bool Colors::appearance(const char * name)
{
	bool result = false;
	String filename_;
	String theme_name;
	if (name == 0 || (name && name[0] == '\0'))
	{
		theme_name = "light";
	}
	else
	{
		theme_name = name;
	}
	filename(theme_name.c_str(),filename_);

	if (File::exists(filename_))
	{
		File file;
		if (file.open(filename_,"rb") != -1)
		{
			Json json;
			try
			{
				json.unserialize(file);
				m_colors_primary.clear();
				m_colors_secondary.clear();

				{
					JsonIterator iterator(json["colors"]);
					for (iterator.first(); iterator.exist(); iterator.next())
					{
						m_colors_primary.push_back(iterator);
						iterator.next();
						m_colors_secondary.push_back(iterator);
					}
				}
				
				theme(m_theme);
				result = true;
			}
			catch(...)
			{
			}
		}
	}
	return result;
}

/** Redefine predefined color */
void Colors::color(uint32_t id, uint32_t color, bool focused)
{
	if (focused)
	{
		if (id < m_colors_secondary.size())
		{
			m_colors_secondary[id] = color;
		}
	}
	else
	{
		if (id < m_colors_primary.size())
		{
			m_colors_primary[id] = color;
		}
	}
}

/** Return the color according to its id 
@param id color identifier
@param focused indicates if the color is focused or not
@return color predefined or id value if not found */
uint32_t Colors::color(uint32_t id, bool focused)
{
	uint32_t result = id;
	if (id <= LAST_THEME_VALUE && id != Color::TRANSPARENT)
	{
		if (focused)
		{
			// If color is in colors table
			if (id < m_colors_secondary.size())
			{
				// Get color
				result = m_colors_secondary[id];
			}
		}
		else
		{
			// If color is in colors table
			if (id < m_colors_primary.size())
			{
				// Get color
				result = m_colors_primary[id];
			}
		}

		// If the color is in theme value table
		if (result >= FIRST_THEME_VALUE && result <= LAST_THEME_VALUE)
		{
			id = (result - FIRST_THEME_VALUE);
			
			// Get theme value
			if (id <= (Color::LAST_THEME_VALUE-Color::FIRST_THEME_VALUE))
			{
				result = m_theme_values[id];
			}
		}
	}
	return result;
}

/** Get style filename according to class name */
void Colors::filename(const char * classname, String & filename_)
{
	filename_.print("colors/%s.json",classname);
}

#ifdef _DEBUG

void Colors::test1()
{
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xFF000000);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("dark") == true);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xffffffff);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("prout") == false);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xffffffff);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("test_light") == true);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xFF000000);
	assert(UIManager::colors()->color(100) == 100);
}

void Colors::test2()
{
}

void Colors::test3()
{
}

void Colors::test4()
{
}

void Colors::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test1();
		test2();
		test3();
		test4();
	}
}
#endif