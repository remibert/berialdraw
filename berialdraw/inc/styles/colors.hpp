#pragma once
namespace berialdraw
{
	class Desktop;
	/** Allows you to manage predefined colors for color theme display */
	class Colors
	{
	public:
		/** Create colors */
		Colors();

		/** Destroy widget */
		~Colors();

		/** Choose the name of the appearance mode
		@param name name of appearance (can be "light", "dark")
		@return true if load with success */
		bool appearance(const char * name=0);

		/** Choose the color theme
		@param color theme */
		void theme(uint32_t color);

		/** Redefine predefined color */
		void color(uint32_t id, uint32_t color, bool focused=false);

		/** Return the color according to its id 
		@param id color identifier
		@param focused indicates if the color is focused or not
		@return color predefined or id value if not found */
		uint32_t color(uint32_t id, bool focused=false);

		/** Get style filename according to class name */
		void filename(const char * classname, String & filename_);

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<uint32_t> m_colors_primary;
		Vector<uint32_t> m_colors_secondary;
		uint32_t m_theme_values[(Color::LAST_THEME_VALUE-Color::FIRST_THEME_VALUE) + 1];
		uint32_t m_theme;
/// @endcond
	};
}
