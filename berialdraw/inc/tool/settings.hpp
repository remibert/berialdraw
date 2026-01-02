#pragma once
namespace berialdraw
{
	/** Hierarchical key-value store for settings using JSON as base
	    Inherits all JSON functionality and adds settings-specific features like resolve() */
	class Settings : public Json
	{
	public:
		/** Constructor */
		Settings();

		/** Constructor with initial JSON buffer
		@param buffer Initial JSON string to parse */
		Settings(const char * buffer);

		/** Destructor */
		virtual ~Settings();

		/** Copy constructor
		@param settings Settings to copy from */
		Settings(const Settings & settings);
	
		/** Assign operator redefinition
		@param other Settings to assign from
		@return Reference to this Settings */
		Settings& operator=(const Settings& other);

		/** Resolve a string by replacing ${name} placeholders with setting values
		@param template_str String containing ${name} placeholders
		@return Resolved string with all placeholders replaced */
		String resolve(const String & template_str);

		/** Resolve a string by replacing ${name} placeholders with setting values
		@param template_str C-string containing ${name} placeholders
		@return Resolved string with all placeholders replaced */
		String resolve(const char * template_str);

/// @cond DOXYGEN_IGNORE
#ifdef _DEBUG
		/** Unitary test */
		static void test(); 
		static void test8();
		static void test7();
		static void test6();
		static void test5();
		static void test4();
		static void test3();
		static void test2();
		static void test1();
#endif
	protected:
#ifndef SWIG

/// @endcond
#endif
	};
}
