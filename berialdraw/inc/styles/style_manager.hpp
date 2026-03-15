#pragma once
namespace berialdraw
{
	/** StyleManager is a global style repository for managing named style presets.
	Attached to UIManager, it provides centralized storage and retrieval of StyleItem objects.
	Independent of any specific widget - styles can be reused across the application. */
	class StyleManager
	{
	public:
		/** Constructor */
		StyleManager();

		/** Destructor */
		virtual ~StyleManager();

		// === STYLE REGISTRATION ===

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

		/** Get all registered style names
		@return Vector of all style names */
		Vector<String> get_style_names() const;

		/** Get the number of registered styles */
		uint32_t style_count() const { return m_items.size(); }

		/** Clear all registered styles */
		void clear();

		/** Serialize all styles to JSON */
		void serialize(JsonIterator & it) const;

		/** Unserialize styles from JSON */
		void unserialize(JsonIterator & it);

	#ifdef _DEBUG
		/** Test 1: Basic style registration and retrieval */
		static void test1();

		/** Test 2: Get all style names */
		static void test2();

		/** Test 3: Unregister/delete a style */
		static void test3();

		/** Test 4: Duplicate registration replaces existing */
		static void test4();

		/** Test 5: Clear all styles */
		static void test5();

		/** Test 6: StyleItem with multiple styles */
		static void test6();

		/** Test 7: StyleItem copy operator */
		static void test7();

		/** Test 8: Serialize and unserialize styles */
		static void test8();

		/** Test 9: Complex style management workflow */
		static void test9();

		/** Main test runner - execute all tests */
		static void test();
	#endif

	protected:
		/// @cond DOXYGEN_IGNORE
		Vector<StyleItem*> m_items;

		/** Find style item index by name
		@param name Name to search for
		@return Index or ~0 if not found */
		uint32_t find_index(const String& name) const;
		/// @endcond
	};
}
