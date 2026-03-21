#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create colors */
Colors::Colors()
{
	m_palette = Color::PALETTE_LIME;
}

/** Destroy widget */
Colors::~Colors()
{
	m_colors_primary.clear();
}

/** Choose the color palette
@param color palette */
void Colors::palette(uint32_t color_palette)
{
	if (color_palette >= Color::FIRST_PALETTE_COLOR && color_palette <= Color::LAST_PALETTE_COLOR)
	{
		uint32_t col;
		m_palette = color_palette;
		col = color(color_palette);

		m_palette_values [Color::PALETTE_BACK_COLOR             - Color::FIRST_PALETTE_VALUE] =  col;

		for (int i = PALETTE_BACK_COLOR_LIGHT_1; i <= PALETTE_BACK_COLOR_LIGHT_15; i++)
		{
			m_palette_values [i-FIRST_PALETTE_VALUE] = Hsl::add_color(col, Color::WHITE, ((i-PALETTE_BACK_COLOR_LIGHT_1) * 17) +8);
		}

		for (int i = PALETTE_BACK_COLOR_DARK_1; i <= PALETTE_BACK_COLOR_DARK_15; i++)
		{
			m_palette_values [i-FIRST_PALETTE_VALUE] = Hsl::add_color(col, Color::BLACK, (i-PALETTE_BACK_COLOR_DARK_1) * 17);
		}

		if (color_palette < m_colors_secondary.size())
		{
			m_palette_values [Color::PALETTE_FORE_COLOR  - Color::FIRST_PALETTE_VALUE] = m_colors_secondary[color_palette];
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
	String palette_name;
	if (name == 0 || (name && name[0] == '\0'))
	{
		palette_name = "light";
	}
	else
	{
		palette_name = name;
	}
	filename(palette_name.c_str(),filename_);

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
				
				m_loaded = true;
				palette(m_palette);
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
	load();
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
	load();
	if (id <= LAST_PALETTE_VALUE && id != Color::TRANSPARENT)
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

		// If the color is in palette value table
		if (result >= FIRST_PALETTE_VALUE && result <= LAST_PALETTE_VALUE)
		{
			id = (result - FIRST_PALETTE_VALUE);
			
			// Get palette value
			if (id <= (Color::LAST_PALETTE_VALUE-Color::FIRST_PALETTE_VALUE))
			{
				result = m_palette_values[id];
			}
		}
	}
	return result;
}

/** Get style filename according to class name */
void Colors::filename(const char * classname, String & filename_)
{
	filename_.print("$(ui.colors)/%s.json",classname);
}

void Colors::load()
{
	if (m_loaded == false)
	{
		m_loaded = true;
		appearance(0);
	}
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