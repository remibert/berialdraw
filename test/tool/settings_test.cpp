#include "berialdraw_imp.hpp"
using namespace berialdraw;

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
	MemoryLeakLog
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

