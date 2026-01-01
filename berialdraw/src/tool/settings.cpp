#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Settings implementation - inherits from Json
Settings::Settings() : Json()
{
	// Create an empty root object
	unserialize("{}");
}

Settings::Settings(const char * buffer) : Json(buffer)
{
	// Constructor with initial buffer
}

Settings::~Settings()
{
	// Json destructor handles cleanup
}

Settings::Settings(const Settings & settings) : Json()
{
	String stream;
	const_cast<Settings&>(settings).serialize(stream, -1);
	unserialize(stream.c_str());
}

Settings& Settings::operator=(const Settings& other)
{
	if (this != &other)
	{
		String stream;
		const_cast<Settings&>(other).serialize(stream, -1);
		unserialize(stream.c_str());
	}
	return *this;
}

String Settings::resolve(const String & template_str)
{
	String result(template_str);
	String var_name;
	int32_t pos = 0;
	
	// Iterate through all variables found in template_str
	do
	{
		pos = template_str.search_var(var_name, pos);
		if (pos != INT32_MAX)
		{
			// Search for the variable value in the JSON object
			JsonIterator it = (*this)[var_name];
			
			if (it.exist())
			{
				String value_str = (String)it;

				// Create placeholder and replace in result
				String placeholder("${");
				placeholder += var_name;
				placeholder += "}";
				result.replace(placeholder.c_str(), value_str.c_str());
			}
			
			// Move past the current variable for next search
			pos += 2; // Move past ${
		}
	}
	while (pos != INT32_MAX);
	
	return result;
}

String Settings::resolve(const char * template_str)
{
	if (template_str)
	{
		String str(template_str);
		return resolve(str);
	}
	return String("");
}

#ifdef _DEBUG
#include <assert.h>

void Settings::test()
{
	// Test basic set/get using JSON API with operator|
	Settings settings;
	settings["debug"] = true;
	JsonIterator it(settings);
	assert((bool)it["debug"] == true);
	assert((bool)(it["debug"] | false) == true);
	assert((bool)(it["nonexistent"] | false) == false);

	// Test basic set/get for integer
	settings["port"] = 8080;
	assert((int32_t)it["port"] == 8080);
	assert((int32_t)(it["port"] | 0) == 8080);
	assert((int32_t)(it["nonexistent"] | 0) == 0);

	// Test basic set/get for string
	settings["name"] = "MyApp";
	assert((String)it["name"] == "MyApp");
	assert((String)(it["name"] | "Default") == "MyApp");
	assert((String)(it["nonexistent"] | "Default") == "Default");

	// Test set with String objects
	String key("hostname");
	String value("localhost");
	settings[key] = value;
	assert((String)it["hostname"] == "localhost");

	// Test existence check
	assert(settings["debug"].exist());
	assert(settings["port"].exist());
	assert(settings["name"].exist());
	assert(settings["hostname"].exist());
	assert(settings["nonexistent"].exist() == false);

	// Test overwrite
	settings["port"] = 9000;
	assert(it["port"] == 9000);
	assert(settings.count() == 4); // Still 4 settings

	// Test remove
	settings["hostname"].remove();  // Remove by setting to null
	assert(settings["hostname"].exist() == false);
	assert(settings.count() == 3);

	// Test size
	Settings s2;
	assert(s2.count() == 0);
	s2["a"] = 1;
	assert(s2.count() == 1);
	s2["b"] = 2;
	assert(s2.count() == 2);

	// Test clear
	s2.clear();
	assert(s2.count() == 0);
	JsonIterator it_s2(s2);
	assert(it_s2["a"].exist() == false);

	// Test resolve with simple placeholder
	Settings s3;
	s3["user"] = "Alice";
	String resolved = s3.resolve("Hello ${user}");
	assert(resolved == "Hello Alice");

	// Test resolve with multiple placeholders
	s3["greeting"] = "Welcome";
	resolved = s3.resolve("${greeting} ${user}!");
	assert(resolved == "Welcome Alice!");

	// Test resolve with UTF-8 characters
	s3["city"] = "Paris";
	resolved = s3.resolve("Je suis à ${city}");
	assert(resolved == "Je suis à Paris");

	// Test resolve complex template
	s3["year"] = 2025;
	s3["month"] = 12;
	resolved = s3.resolve("Date: ${year}-${month}");
	assert(resolved == "Date: 2025-12");

	// Test copy constructor
	Settings s4(s3);
	JsonIterator it4(s4);
	assert((String)it4["user"] == "Alice");
	assert((int32_t)it4["year"] == 2025);

	// Test assignment operator
	Settings s5;
	s5 = s3;
	JsonIterator it5(s5);
	assert((String)it5["city"] == "Paris");

	// Test serialize/unserialize
	String stream;
	s3.serialize(stream, -1);
	Settings s6;
	s6.unserialize(stream.c_str());
	JsonIterator it6(s6);
	assert((String)it6["user"] == "Alice");
	assert((String)it6["greeting"] == "Welcome");
	assert((int32_t)it6["year"] == 2025);

	// Test resolve with empty string
	resolved = s3.resolve("");
	assert(resolved == "");

	// Test resolve with no placeholders
	resolved = s3.resolve("No placeholders here");
	assert(resolved == "No placeholders here");

	// Test serialize/unserialize with formatted output
	String stream_fmt;
	s3.serialize(stream_fmt, 0);
	assert(stream_fmt.find("\"user\"") > 0); // Should have formatted JSON
	assert(stream_fmt.find("\"Alice\"") > 0);

	// Test JSON API features inherited by Settings
	Settings s7;
	s7["count"] = 42;
	s7["enabled"] = true;
	s7["message"] = "Test";
	JsonIterator it7(s7);
	// Test operator| for defaults
	assert((int32_t)(it7["count"] | 0) == 42);
	assert((bool)(it7["enabled"] | false) == true);
	assert((String)(it7["message"] | "None") == "Test");
	assert((int32_t)(it7["missing"] | 999) == 999);
	assert((String)(it7["absent"] | "Default") == "Default");
}

#endif
