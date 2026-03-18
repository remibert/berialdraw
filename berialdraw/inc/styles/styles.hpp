#pragma once
namespace berialdraw
{
	class Desktop;
	class Widget;
	class StyleItem;

	/** Cache for managing JSON file loading with lazy loading and smart reuse */
	class JsonFileCache
	{
	public:
		JsonFileCache();
		~JsonFileCache();

		/** Load JSON from file if not already loaded, or return existing cache if same filename */
		std::unique_ptr<JsonIterator> get_iterator(const String & filename);

		/** Clear the cache and free resources */
		void clear();

	protected:
		Json * m_json = 0;
		String m_filename;
	};

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

		/** Apply style properties to a widget
		@param classname The class name of the widget
		@param properties Pointer to the style properties to be filled
		@return true if style was successfully applied */
		template<typename T>
		bool apply(const char* classname, T* properties)
		{
			if (!classname || !properties) return false;
			
			auto creator = []() -> Style* { return T::create(); };
			T* style = dynamic_cast<T*>(select(classname, properties->property_name(), creator));
			if (style)
			{
				*properties = *style;
				return true;
			}
			return false;
		}

		/** Apply style properties to a widget via Widget pointer
		@param widget The widget instance to get classname from
		@param properties Pointer to the style properties to be filled
		@return true if style was successfully applied */
		template<typename T>
		bool apply(Widget* widget, T* properties);

		/** Template specialization for Key to resolve ambiguous 'create()' from multiple inheritance */
		template<>
		inline bool apply<Key>(const char* classname, Key* properties)
		{
			if (!classname || !properties) return false;
			
			auto creator = []() -> Style* { return CommonStyle::create(); };
			Key* style = dynamic_cast<Key*>(select(classname, properties->property_name(), creator));
			if (style)
			{
				*properties = *style;
				return true;
			}
			return false;
		}

		/** Get mappings properties */
		const Mappings * mappings(const char * classname);

		/** Get keys properties */
		const Keys * keys(const char * classname);

		/** Get style name */
		const String & style();

		/** Set style name */
		void style(const String & name);


		/** Add a style with a name and properties (creates new StyleItem)
		@param name Name of the style
		@param properties JSON string of properties */
		void add_style(const String& name, const String& properties);

		/** Remove a style by name
		@param name Name of the style to remove
		@return true if removed, false if not found */
		bool remove_style(const String& name);

		/** Get a registered style by name
		@param name Name of the style
		@return Pointer to StyleItem or nullptr if not found */
		StyleItem* get_style(const String& name) const;

		/** Check if a style exists
		@param name Name of the style
		@return true if style exists */
		bool has_style(const String& name) const;



#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Find style item index by name */
		uint32_t find_index(const String& name) const;

		/** Select the style according to the name specified, load it if not yet existing */
		Style * select(const char * classname, const char * property_name, StyleCreator_t creator);

		/** Load the style according to the name specified */
		Style * load(const char * classname, const char * property_name, StyleCreator_t creator);

		Vector<StyleItem*>    m_items;
		JsonFileCache        m_json_cache;
		String m_style;
/// @endcond
	};
}
