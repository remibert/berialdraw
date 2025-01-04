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
	if (col >= Color::FIRST_THEME && col <= Color::LAST_THEME)
	{
		m_theme = col;
		col = color(col);
		uint32_t front_gray_level = Hsl::to_gray(col)&0xFF;
		uint32_t back_gray_level = Hsl::to_gray(color(Color::WINDOW_COLOR)) & 0xFF;

		color(Color::VALUE_THEME_BACK_COLOR,     col);
		color(Color::VALUE_THEME_SELECT_COLOR,   back_gray_level > 128 ? col & 0x40FFFFFF : col & 0x90FFFFFF);
		color(Color::VALUE_THEME_FOCUS_COLOR,    Hsl::adjust_light(col, -10));
		color(Color::VALUE_THEME_FUNC_KEY_COLOR, Hsl::to_gray(col));
		color(Color::VALUE_THEME_FRONT_COLOR,    (front_gray_level >= 160 ? Color::BLACK: Color::WHITE));

		uint32_t track_gray_color = (front_gray_level+back_gray_level) >> 1;
		track_gray_color = 0xFF000000 | (track_gray_color & 0xFF) | ((track_gray_color & 0xFF) << 8) | ((track_gray_color & 0xFF) << 16);
		color(Color::VALUE_THEME_TRACK_COLOR, track_gray_color);
		color(Color::VALUE_THEME_FUNC_KEY_COLOR, back_gray_level > 128 ? col & 0xA0FFFFFF : col & 0x90FFFFFF);
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
				JsonIterator iterator(json);
				for (iterator.first(); iterator.exist(); iterator.next())
				{
					m_colors.push_back(iterator);
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
	uint32_t result = id;
	if (id < m_colors.size())
	{
		result = m_colors[id];
		if (result > Color::TRANSPARENT && result < m_colors.size())
		{
			result = m_colors[result];
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
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xFFC0C0C0);
	assert(UIManager::colors()->color(100) == 100);

	assert(UIManager::colors()->appearance("prout") == false);
	assert(UIManager::colors()->color(LABEL_TEXT_COLOR) == 0xFFC0C0C0);
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