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
	String resolved(template_str);
	String var_name;
	int32_t pos = 0;
	bool found;
	int32_t count = 0;

	do
	{
		found = false;
		
		// Iterate through all variables found in template_str
		do
		{
			pos = resolved.search_var(var_name, pos);
			if (pos != INT32_MAX)
			{
				// Parse dotted notation (e.g., "toto.titi.tata")
				JsonIterator it(*this);
			
				// Split variable name by dots and navigate through nested structure
				int32_t dot_pos = 0;
				int32_t next_dot = 0;
				String current_key;
				bool var_found = true;
			
				while (dot_pos < (int32_t)var_name.count() && var_found)
				{
					// Find next dot or end of string
					next_dot = var_name.find(".", dot_pos);
					if (next_dot == INT32_MAX)
					{
						next_dot = var_name.count();
					}
				
					// Extract key between dot_pos and next_dot
					var_name.slice(dot_pos, next_dot, current_key);
				
					// Navigate to nested value
					it = it[current_key.c_str()];
				
					// Check if the path exists
					if (!it.exist())
					{
						var_found = false;
						break;
					}
				
					// Move to next segment
					dot_pos = next_dot + 1;
				}
			
				// If we found the complete path, replace it
				if (var_found && it.exist())
				{
					String value_str = (String)it;

					// Create placeholder and replace in result
					String placeholder("$(");
					placeholder += var_name;
					placeholder += ")";
					result.replace(placeholder.c_str(), value_str.c_str());
					found = true;
				}
			
				// Move past the current variable for next search
				pos += 2; // Move past $(
			}
		}
		while (pos != INT32_MAX);

		resolved = result;
		pos = 0;
		count ++;
	}
	while (found == true && count < 3);
	
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

void Settings::test1()
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
}

void Settings::test2()
{
	// Test set with String objects and existence checks
	Settings settings;
	String key("hostname");
	String value("localhost");
	settings[key] = value;
	JsonIterator it(settings);
	assert((String)it["hostname"] == "localhost");

	// Test existence check
	assert(settings["hostname"].exist());
	assert(settings["nonexistent"].exist() == false);

	// Test overwrite
	settings["hostname"] = "127.0.0.1";
	assert((String)it["hostname"] == "127.0.0.1");

	// Test remove
	settings["hostname"].remove();
	assert(settings["hostname"].exist() == false);
}

void Settings::test3()
{
	// Test size and clear operations
	Settings s1;
	assert(s1.count() == 0);
	s1["a"] = 1;
	assert(s1.count() == 1);
	s1["b"] = 2;
	assert(s1.count() == 2);
	s1["c"] = 3;
	assert(s1.count() == 3);

	// Test clear
	s1.clear();
	assert(s1.count() == 0);
	JsonIterator it_s1(s1);
	assert(it_s1["a"].exist() == false);
}

void Settings::test4()
{
	// Test resolve with simple and multiple placeholders
	Settings s;
	s["user"] = "Alice";
	String resolved = s.resolve("Hello $(user)");
	assert(resolved == "Hello Alice");

	// Test resolve with multiple placeholders
	s["greeting"] = "Welcome";
	resolved = s.resolve("$(greeting) $(user)!");
	assert(resolved == "Welcome Alice!");

	// Test resolve with UTF-8 characters
	s["city"] = "Paris";
	resolved = s.resolve("Je suis à $(city)");
	assert(resolved == "Je suis à Paris");

	// Test resolve complex template
	s["year"] = 2025;
	s["month"] = 12;
	resolved = s.resolve("Date: $(year)-$(month)");
	assert(resolved == "Date: 2025-12");
}

void Settings::test5()
{
	// Test copy constructor and assignment operator
	Settings s1;
	s1["user"] = "Alice";
	s1["port"] = 8080;
	s1["active"] = true;

	// Test copy constructor
	Settings s2(s1);
	JsonIterator it2(s2);
	assert((String)it2["user"] == "Alice");
	assert((int32_t)it2["port"] == 8080);
	assert((bool)it2["active"] == true);

	// Test assignment operator
	Settings s3;
	s3 = s1;
	JsonIterator it3(s3);
	assert((String)it3["user"] == "Alice");
	assert((int32_t)it3["port"] == 8080);
}

void Settings::test6()
{
	// Test serialize/unserialize
	Settings s1;
	s1["user"] = "Alice";
	s1["greeting"] = "Welcome";
	s1["year"] = 2025;
	s1["active"] = true;

	String stream;
	s1.serialize(stream, -1);
	
	Settings s2;
	s2.unserialize(stream.c_str());
	JsonIterator it(s2);
	assert((String)it["user"] == "Alice");
	assert((String)it["greeting"] == "Welcome");
	assert((int32_t)it["year"] == 2025);
	assert((bool)it["active"] == true);

	// Test serialize/unserialize with formatted output
	String stream_fmt;
	s1.serialize(stream_fmt, 0);
	assert(stream_fmt.find("\"user\"") > 0);
	assert(stream_fmt.find("\"Alice\"") > 0);
}

void Settings::test7()
{
	// Test JSON API features inherited by Settings with operator| defaults
	Settings s;
	s["count"] = 42;
	s["enabled"] = true;
	s["message"] = "Test";
	JsonIterator it(s);
	
	// Test operator| for defaults
	assert((int32_t)(it["count"] | 0) == 42);
	assert((bool)(it["enabled"] | false) == true);
	assert((String)(it["message"] | "None") == "Test");
	assert((int32_t)(it["missing"] | 999) == 999);
	assert((String)(it["absent"] | "Default") == "Default");

	// Test resolve with empty string
	String resolved = s.resolve("");
	assert(resolved == "");

	// Test resolve with no placeholders
	resolved = s.resolve("No placeholders here");
	assert(resolved == "No placeholders here");
}

void Settings::test8()
{
	// Test nested JSON with dotted notation $(toto.titi.tata)
	Settings config;
	
	// Create nested structure: config["database"]["host"] = "localhost"
	config["database"]["host"] = "localhost";
	config["database"]["port"] = 5432;
	config["database"]["username"] = "admin";
	config["database"]["password"] = "secret123";
	
	// Create another nested level: config["app"]["name"] = "MyApp"
	config["app"]["name"] = "MyApp";
	config["app"]["version"] = "1.0";
	config["app"]["debug"] = true;
	
	// Create deeper nesting: config["server"]["ssl"]["enabled"] = true
	config["server"]["ssl"]["enabled"] = true;
	config["server"]["ssl"]["cert"] = "/path/to/cert.pem";
	config["server"]["ssl"]["key"] = "/path/to/key.pem";
	
	// Test single level access (still works)
	String resolved = config.resolve("Database: $(database)");
	// Note: This will not replace since database is an object, not a scalar
	
	// Test two-level nested access
	resolved = config.resolve("Host: $(database.host)");
	assert(resolved == "Host: localhost");
	
	resolved = config.resolve("Port: $(database.port)");
	assert(resolved == "Port: 5432");
	
	// Test three-level nested access
	resolved = config.resolve("SSL Enabled: $(server.ssl.enabled)");
	assert(resolved == "SSL Enabled: true"); // boolean converted to "1"
	
	resolved = config.resolve("Cert Path: $(server.ssl.cert)");
	assert(resolved == "Cert Path: /path/to/cert.pem");
	
	// Test multiple placeholders with different nesting levels
	resolved = config.resolve("Connecting to $(database.username)@$(database.host):$(database.port)");
	assert(resolved == "Connecting to admin@localhost:5432");
	
	// Test complex template with mixed nesting
	resolved = config.resolve("$(app.name) v$(app.version) - DB: $(database.host) (SSL: $(server.ssl.enabled))");
	assert(resolved == "MyApp v1.0 - DB: localhost (SSL: true)");
	
	// Test missing nested path (should not replace)
	resolved = config.resolve("Missing: $(database.missing.path)");
	assert(resolved == "Missing: $(database.missing.path)"); // Placeholder unchanged
	
	// Test partial missing path (should not replace)
	resolved = config.resolve("Partial: $(database.nonexistent)");
	assert(resolved == "Partial: $(database.nonexistent)"); // Placeholder unchanged
}

void Settings::test()
{
	// Run all tests starting from the highest number
	test8();  // Test nested dotted notation
	test7();  // Test JSON API features and operator| defaults
	test6();  // Test serialize/unserialize
	test5();  // Test copy constructor and assignment
	test4();  // Test resolve operations
	test3();  // Test size and clear
	test2();  // Test set with String objects
	test1();  // Test basic set/get
}

#endif
