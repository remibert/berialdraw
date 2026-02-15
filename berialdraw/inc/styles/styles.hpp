#pragma once
namespace berialdraw
{
	class Desktop;

	/** The Styles class manages a collection of Style instances, from which all specific style classes 
	(e.g., BorderStyle, SliderStyle) inherit. 
	It acts as a centralized container for loading, organizing, and selecting styles, typically sourced 
	from configuration files. This allows dynamic customization of widget appearances and facilitates 
	theme management across the UI. */
	class Styles
	{
	public:
		/** Create styles */
		Styles();

		/** Destroy widget */
		~Styles();

		/** Clear styles */
		void clear();

		/** Apply common properties */
		bool apply(const char * classname, CommonStyle * properties);

		/** Apply widget properties */
		bool apply(const char * classname, WidgetStyle * properties);

		/** Apply text properties */
		bool apply(const char * classname, TextStyle * properties);

		/** Apply border properties */
		bool apply(const char * classname, BorderStyle * properties);

		/** Apply edit properties */
		bool apply(const char * classname, EditStyle * properties);

		/** Apply switch properties */
		bool apply(const char * classname, SwitchStyle * properties);

		/** Apply checkbox properties */
		bool apply(const char * classname, CheckboxStyle * properties);

		/** Apply radio properties */
		bool apply(const char * classname, RadioStyle * properties);

		/** Apply slider properties */
		bool apply(const char * classname, SliderStyle * properties);

		/** Apply progress properties */
		bool apply(const char * classname, ProgressBarStyle * properties);

		/** Apply icon properties */
		bool apply(const char * classname, IconStyle * properties);
		
		/** Apply scrollview properties */
		bool apply(const char * classname, ScrollViewStyle * properties);
		
		/** Apply tableview properties */
		bool apply(const char * classname, TableViewStyle * properties);
		
		/** Get mappings properties */
		const Mappings * mappings(const char * classname);

		/** Get keys properties */
		const Keys * keys(const char * classname);

		/** Get style name */
		const String & style();

		/** Set style name */
		void style(const String & name);

		/** Get style filename according to classname */
		void filename(const char * classname, String & filename);

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Select the style according to the name specified, load it if not yet existing */
		Style * select(const char * name, const char * properties, StyleCreator_t creator);

		/** Load the style according to the name specified */
		Style * load(const char * name, const char * properties, StyleCreator_t creator);

		Vector<String * >    m_names;
		Vector<Style *>      m_styles;
		Json * m_json = 0;
		String m_json_name;
		String m_style;
/// @endcond
	};
}
