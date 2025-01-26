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
	m_colors.clear();
}


/** Choose the color theme
@param color theme */
void Colors::theme(uint32_t col)
{
	if (col >= Color::FIRST_THEME_COLOR && col <= Color::LAST_THEME_COLOR)
	{
		m_theme = col;
		col = color(col);

		m_theme_values [Color::THEME_COLOR_XXX_LIGHT   - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::WHITE, 204); // Transparency 80%
		m_theme_values [Color::THEME_COLOR_XX_LIGHT    - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::WHITE, 154); // Transparency 60%
		m_theme_values [Color::THEME_COLOR_X_LIGHT     - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::WHITE, 102); // Transparency 40%
		m_theme_values [Color::THEME_COLOR_LIGHT       - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::WHITE, 51);  // Transparency 20%
		m_theme_values [Color::THEME_COLOR             - Color::FIRST_THEME_VALUE] =  col;
		m_theme_values [Color::THEME_COLOR_DARK        - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::BLACK, 44);  // Transparency 17%
		m_theme_values [Color::THEME_COLOR_X_DARK      - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::BLACK, 87);  // Transparency 34%
		m_theme_values [Color::THEME_COLOR_XX_DARK     - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::BLACK, 131); // Transparency 51%
		m_theme_values [Color::THEME_COLOR_XXX_DARK    - Color::FIRST_THEME_VALUE] =  Hsl::add_color(col, Color::BLACK, 174); // Transparency 68%

		// If light theme selected
		if (Hsl::delta_contrast(col, Color::WHITE) > Hsl::delta_contrast(col, Color::BLACK))
		{
			m_theme_values [Color::THEME_COLOR_FOREGROUND  - Color::FIRST_THEME_VALUE] = Color::WHITE;
		}
		else
		{
			m_theme_values [Color::THEME_COLOR_FOREGROUND  - Color::FIRST_THEME_VALUE] = Color::BLACK;
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
				m_colors.clear();
				m_state_colors.clear();

				{
					JsonIterator iterator(json["colors"]);
					for (iterator.first(); iterator.exist(); iterator.next())
					{
						m_colors.push_back(iterator);
					}
				}
				
				{
					JsonIterator iterator = json["state-colors"];

					for (iterator.first(); iterator.exist(); iterator.next())
					{
						m_state_colors.push_back(iterator);
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
void Colors::color(uint32_t id, uint32_t color)
{
	if (id < m_colors.size())
	{
		m_colors[id] = color;
	}
}

/** Return the color according to its id 
@param id color identifier
@return color predefined or id value if not found */
uint32_t Colors::color(uint32_t id)
{
	return color(id, false);
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
		// If color is in colors table
		if (id < m_colors.size())
		{
			// Get color
			result = m_colors[id];
		}

		// If state color selected
		if (result >= FIRST_STATE_COLOR && result <= LAST_STATE_COLOR)
		{
			id = (result - FIRST_STATE_COLOR) * 2;
			if (focused)
			{
				id += 1;
			}
			if (id < m_state_colors.size())
			{
				result = m_state_colors[id];
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
	filename_.print("resources/colors/%s.json",classname);
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
	test1();
	test2();
	test3();
	test4();
}
#endif