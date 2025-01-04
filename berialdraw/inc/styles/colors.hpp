#ifndef Colors_hpp_INCLUDED
#define Colors_hpp_INCLUDED

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
		void color(uint32_t id, uint32_t color);

		/** Return the color according to its id 
		@param id color identifier
		@return color predefined or id value if not found */
		uint32_t color(uint32_t id);

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
		Vector<uint32_t> m_colors;
		uint32_t m_theme;
/// @endcond
	};
}
#endif
